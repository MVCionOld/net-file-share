#include "CSender.hpp"


CSender::CSender (std::string ip)
    : ip_(std::move(ip)) {}

CSender::~CSender () {
  close_socksfd(sockfd_);
}

void CSender::Send (std::string file_path, size_t threads_amt) {
  file_path_ = std::move(file_path);
  threads_amt_ = threads_amt;
  const int source_fd = open_file(file_path_.c_str());
  const auto file_size = get_file_size(source_fd);
  auto source_map = map_file_r(source_fd, file_size);
  uint16_t port;
  makeHandshake(port);
  if (sockfd_ == FdTypeVal::ERROR_FD) {
    unmap_file(source_map, file_size);
    return;
  }
  std::vector<uint16_t> ports = {port};
  std::vector<int> sockfds = {sockfd_};
  setUpConnection(source_fd, ports);
  close_file(source_fd);
  std::vector<std::thread> senders;
  const auto block_size = file_size / threads_amt_;
  for (size_t thread_id = 1; thread_id < threads_amt_; ++thread_id) {
    sockfds.emplace_back(
        get_ready_socksfd(ip_.c_str(), ports[thread_id])
    );
  }
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    senders.emplace_back(
        [&] (int sender_id) {
          auto pkg_amt = (block_size + PACKAGE_SIZE - 1) / PACKAGE_SIZE;
          byte package[2 * PACKAGE_SIZE];
          for (size_t pkg_id = 0; pkg_id < pkg_amt; ++pkg_id) {
            auto package_size = static_cast<size_t>(PACKAGE_SIZE);
            if (pkg_id == pkg_amt - 1) {
              if (sender_id == threads_amt_ - 1) {
                package_size = file_size - block_size * sender_id
                               - PACKAGE_SIZE * pkg_id;
              } else {
                package_size = block_size - PACKAGE_SIZE * pkg_id;
              }
            }
            read_mmap(
                package,
                source_map,
                package_size,
                sender_id * block_size + pkg_id * PACKAGE_SIZE
            );
            write_package(
                sockfds[sender_id],
                package,
                package_size
            );
          }
        },
        thread_id);
  }
  for (auto &sender: senders) {
    sender.join();
  }
  for (auto socksfd: sockfds) {
    close_socksfd(socksfd);
  }
  unmap_file(source_map, file_size);
}

void CSender::setUpConnection (int fd, std::vector<uint16_t> &ports) {
  const size_t file_size_off = sizeof(size_t);
  const size_t file_nm_off = sizeof(size_t) + sizeof(uint64_t);
  /*
    Handshake
    {
      size_t   ports_amt
      uint64_t file_size
      string   file_nm
    };
 */
  SendHandshakeBuff send_buff;
  parse(
      send_buff.buffer,
      reinterpret_cast<byte *>(&threads_amt_),
      sizeof(threads_amt_)
  );
  auto file_size = get_file_size(fd);
  parse(
      send_buff.buffer + file_size_off,
      reinterpret_cast<byte *>(&file_size),
      sizeof(file_size)
  );
  get_file_name(
      file_path_.c_str(),
      send_buff.buffer + file_nm_off,
      FILENAME_MAX
  );
  write_package(
      sockfd_,
      reinterpret_cast<void *>(&send_buff),
      sizeof(SendHandshakeBuff)
  );
  RecvHandshakeBuff recv_buff;
  read_package(
      sockfd_,
      reinterpret_cast<void *>(&recv_buff),
      sizeof(RecvHandshakeBuff)
  );
  threads_amt_ = recv_buff.ports_amt; // if receiver hasn't enough ports
  for (size_t port_idx = 1; port_idx < threads_amt_; ++port_idx) {
    ports.emplace_back(recv_buff.ports[port_idx]);
  }
}


void CSender::makeHandshake (uint16_t &port) {
  for (port = PortRange::FROM; port < PortRange::TO; ++port) {
    sockfd_ = get_ready_socksfd(ip_.c_str(), port);
    if (sockfd_ == FdTypeVal::ERROR_FD) {
      continue;
    }
    constexpr size_t cntl_size = HandshakeVal::CONTROL_CODE_SIZE;
    byte control_code[cntl_size] = {0};
    std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> distribution{-128, 127};
    std::generate(
        control_code,
        control_code + cntl_size,
        [&distribution, &generator] () {
          return static_cast<byte>(distribution(generator));
        });
    write_package(sockfd_, control_code, cntl_size);
    byte receive_code[cntl_size] = {0};
    read_package(sockfd_, receive_code, cntl_size);
    for (size_t i = 0; i < cntl_size; ++i) {
      if (control_code[i] != receive_code[i]) {
        close(sockfd_);
        sockfd_ = FdTypeVal::ERROR_FD;
        continue;
      }
    }
    return;
  }
}
