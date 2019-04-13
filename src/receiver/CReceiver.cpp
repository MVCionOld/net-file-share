#include "CReceiver.hpp"


CReceiver::CReceiver () {
  port_ = choose_available_port(PortRange::FROM, PortRange::TO);
  sockfd_ = get_ready_sockrfd(port_);
}

CReceiver::~CReceiver () {
  close_sockrfd(sockfd_);
}

void CReceiver::Receive () {
  int clifd = accept_clientfd(sockfd_);
  std::vector<uint16_t> ports;
  std::string file_nm;
  uint64_t file_size;
  std::vector<int> sockfds = {sockfd_}, clifds = {clifd};
  std::vector<std::thread> accepters;
  setUpConnection(clifd, ports, sockfds, clifds, file_nm, file_size, accepters);
  int dest_fd = prepareFile(file_nm, file_size);
  auto dest_map = map_file_w(dest_fd, file_size);
  close_file(dest_fd);
  const auto receiver_size = file_size / threads_amt_;
  std::vector<std::thread> receivers;
  auto receiver_action =
      [receiver_size, dest_map, file_size, &clifds] (size_t thread_id, size_t threads_amt) {
        auto pkg_amt = static_cast<size_t>(receiver_size / PACKAGE_SIZE);
        byte package[PACKAGE_SIZE];
        for (size_t pkg_id = 0; pkg_id < pkg_amt; ++pkg_id) {
          auto package_size = static_cast<size_t>(PACKAGE_SIZE);
          if (pkg_id == pkg_amt - 1) {
            if (thread_id == threads_amt - 1) {
              package_size = file_size - receiver_size * thread_id
                             - PACKAGE_SIZE * pkg_id;
            } else {
              package_size = receiver_size - PACKAGE_SIZE * pkg_id;
            }
          }
          read_package(
              clifds[thread_id],
              package,
              package_size
          );
          write_mmap(
              dest_map,
              package,
              package_size,
              thread_id * receiver_size + pkg_id * PACKAGE_SIZE
          );
        }
      };
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    receivers.emplace_back(receiver_action, thread_id, threads_amt_);
  }
  for (auto &accepter: accepters) {
    accepter.join();
  }
  for (auto &receiver: receivers) {
    if (receiver.joinable()) {
      receiver.join();
    }
  }
  for (auto clientfd: clifds) {
    close_sockrfd(clientfd);
  }
  for (auto sockfd: sockfds) {
    close_sockrfd(sockfd);
  }
  unmap_file(dest_map, file_size);
}


void CReceiver::setUpConnection (int clifd, std::vector<uint16_t> &ports,
                                 std::vector<int> &sockfds, std::vector<int> &clifds,
                                 std::string &file_nm, uint64_t &file_size,
                                 std::vector<std::thread> &accepters) {
  SendHandshakeBuff buff;
  read_package(
      clifd,
      reinterpret_cast<void *>(buff.buffer),
      HandshakeVal::HANDSHAKE_SIZE
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
  recv_buff.ports[0] = port_;
  ports.emplace_back(recv_buff.ports[0]);
  for (size_t port_idx = 1; port_idx < threads_amt_; ++port_idx) {
    recv_buff.ports[port_idx] = choose_available_port(
        PortRange::FROM,
        PortRange::TO
    );
    ports.emplace_back(recv_buff.ports[port_idx]);
    sockfds.emplace_back(get_ready_sockrfd(ports[port_idx]));
  }
  for (size_t port_idx = 1; port_idx < threads_amt_; ++port_idx) {
    accepters.emplace_back(
        [&] (size_t thread_idx) {
          clifds.emplace_back(accept_clientfd(sockfds[thread_idx]));
        },
        port_idx
    );
  }
  write_package(
      clifd,
      reinterpret_cast<void *>(&recv_buff),
      sizeof(recv_buff)
  );
}

int CReceiver::prepareFile (const std::string &file_nm, uint64_t file_size) {
  int fd = create_file(file_nm.c_str());
  file_truncate(fd, file_size);
  return fd;
}
