#include "CReceiver.hpp"


CReceiver::CReceiver () {
  activated_port_ = choose_available_port(PortRange::FROM, PortRange::TO);
  sockfd_ = get_ready_sockrfd(activated_port_);
}

CReceiver::~CReceiver () {
  close_sockrfd(sockfd_);
}

void CReceiver::Receive () {
  if (sockfd_ == FdTypeVal::ERROR_FD) {
    return;
  }
  int clifd = makeHandshake();
  std::string file_nm;
  uint64_t file_size;
  std::vector<int> sockfds, clifds;
  std::vector<std::thread> accepters;
  setUpConnection(clifd, sockfds, clifds, file_nm, file_size, accepters);
  int dest_fd = prepareFile(file_nm, file_size);
  auto dest_map = map_file_w(dest_fd, file_size);
  close_file(dest_fd);
  const auto block_size = (file_size + threads_amt_ - 1) / threads_amt_;
  std::vector<std::thread> receivers;

  auto receiver_action =
      [block_size, dest_map, file_size, clifds] (size_t receiver_id, size_t threads_amt) {
        auto pkg_amt = (block_size + PACKAGE_SIZE - 1) / PACKAGE_SIZE;
        if (receiver_id == threads_amt - 1) {
          const auto last_block = file_size - block_size * receiver_id;
          pkg_amt = (last_block + PACKAGE_SIZE - 1) / PACKAGE_SIZE;
        }
        byte package[2 * PACKAGE_SIZE]; // to prevent 'stack smashed' if package_size bigger
        for (size_t pkg_id = 0; pkg_id < pkg_amt; ++pkg_id) {
          auto package_size = static_cast<size_t>(PACKAGE_SIZE);
          if (pkg_id == pkg_amt - 1) {
            if (receiver_id == threads_amt - 1) {
              package_size = file_size - block_size * receiver_id
                             - PACKAGE_SIZE * pkg_id;
            } else {
              package_size = block_size - PACKAGE_SIZE * pkg_id;
            }
          }
          //fprintf(stdout, "stage: %d; receiver: %d; package: %d / %d\n", 0, receiver_id, pkg_id, pkg_amt);
          //fflush(stdout);
          read_package(
              clifds[receiver_id],
              package,
              package_size
          );
          //fprintf(stdout, "stage: %d; receiver: %d; package: %d\n", 1, receiver_id, pkg_id);
          //fflush(stdout);
          write_mmap(
              dest_map,
              package,
              package_size,
              receiver_id * block_size + pkg_id * PACKAGE_SIZE
          );
          fprintf(stdout, "stage: %d; receiver: %d; package: %d\n", 2, receiver_id, pkg_id);
          fflush(stdout);
        }
      };

  for (auto &accepter: accepters) {
    accepter.join();
  }
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    receivers.emplace_back(receiver_action, thread_id, threads_amt_);
  }
  for (auto &receiver: receivers) {
    receiver.join();
  }
  for (auto clientfd: clifds) {
    close_sockrfd(clientfd);
  }
  close_sockrfd(clifd);
  for (auto sockfd: sockfds) {
    close_sockrfd(sockfd);
  }
  close_sockrfd(sockfd_);
  unmap_file(dest_map, file_size);
}


void CReceiver::setUpConnection (int clifd, std::vector<int> &sockfds,
                                 std::vector<int> &clifds,
                                 std::string &file_nm, uint64_t &file_size,
                                 std::vector<std::thread> &accepters) {
  printf("Establishing connection...\n");
  fflush(stdout);
  SendHandshakeBuff buff;
  read_package(
      clifd,
      reinterpret_cast<void *>(buff.buffer),
      sizeof(SendHandshakeBuff)
  );
  /*
    Handshake
    {
      size_t   ports_amt
      uint64_t file_size
      string   file_nm
    };
 */
  const size_t file_size_off = sizeof(size_t);
  const size_t file_nm_off = sizeof(size_t) + sizeof(uint64_t);
  parse(
      reinterpret_cast<byte *>(&threads_amt_),
      buff.buffer,
      sizeof(threads_amt_)
  );
  parse(
      reinterpret_cast<byte *>(&file_size),
      buff.buffer + file_size_off,
      sizeof(file_size)
  );
  file_nm = std::move(std::string(buff.buffer + file_nm_off));
  RecvHandshakeBuff recv_buff{};
  recv_buff.ports_amt = threads_amt_;
  for (size_t port_idx = 0; port_idx < threads_amt_; ++port_idx) {
    recv_buff.ports[port_idx] = choose_available_port(
        ++activated_port_,
        PortRange::TO
    );
    activated_port_ = recv_buff.ports[port_idx];
    sockfds.emplace_back(get_ready_sockrfd(recv_buff.ports[port_idx]));
  }

  for (size_t port_idx = 0; port_idx < threads_amt_; ++port_idx) {
    accepters.emplace_back(
        [&] (size_t thread_idx) {
          std::lock_guard<std::mutex> lock(ports_mutex);
          clifds.emplace_back(accept_clientfd(sockfds[thread_idx]));
        },
        port_idx
    );
  }
  write_package(
      clifd,
      reinterpret_cast<void *>(&recv_buff),
      sizeof(RecvHandshakeBuff)
  );
  printf("Connection established!\n");
  fflush(stdout);
}

int CReceiver::makeHandshake () {
  int clifd = accept_clientfd(sockfd_);
  byte control_code[HandshakeVal::CONTROL_CODE_SIZE];
  read_package(clifd, control_code, HandshakeVal::CONTROL_CODE_SIZE);
  write_package(clifd, control_code, HandshakeVal::CONTROL_CODE_SIZE);
  return clifd;
}

int CReceiver::prepareFile (const std::string &file_nm, uint64_t file_size) {
  int fd = create_file(file_nm.c_str());
  file_truncate(fd, file_size);
  return fd;
}
