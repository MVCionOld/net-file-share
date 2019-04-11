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
  auto file_size = get_file_size(source_fd);
  mmap_t source_map = map_file_r(source_fd, file_size);
  close_file(source_fd);
  const uint16_t port = choose_port(
      ip_.c_str(),
      PortRange::FROM,
      PortRange::TO
  );
  std::vector<uint16_t> ports = {port};
  sockfd_ = get_ready_socksfd(ip_.c_str(), port);
  makeHandshake(sockfd_, ports);
  std::vector<int> sockfds = {sockfd_};
  std::vector<std::thread> senders;
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    if (thread_id != 0) {
      const int socksfd = get_ready_socksfd(ip_.c_str(), ports[thread_id]);
      sockfds.emplace_back(socksfd);
    }
    const auto sender_size = file_size / threads_amt_;
    senders.emplace_back([&] () {
      auto pkg_amt = static_cast<size_t>(sender_size / PACKAGE_SIZE);
      byte package[PACKAGE_SIZE];
      for (size_t pkg_id = 0; pkg_id < pkg_amt; ++pkg_id) {
        auto package_size = static_cast<size_t>(PACKAGE_SIZE);
        if (pkg_id == pkg_amt - 1) {
          if (thread_id == threads_amt_ - 1) {
            package_size = file_size - sender_size * thread_id
                           - PACKAGE_SIZE * pkg_id;
          } else {
            package_size = sender_size - PACKAGE_SIZE * pkg_id;
          }
        }
        read_mmap(
            package,
            source_map,
            package_size,
            thread_id * sender_size + pkg_id * PACKAGE_SIZE
        );
        write_package(
            sockfds[thread_id],
            package,
            package_size
        );
      }
    });
  }
  for (auto &sender: senders) {
    sender.join();
  }
  for (auto socksfd: sockfds) {
    close_socksfd(socksfd);
  }
  unmap_file(source_map, file_size);
}

void CSender::makeHandshake (int fd, std::vector<uint16_t> &ports) {
  const size_t threads_amt_off = sizeof(size_t);
  const size_t file_size_off = sizeof(size_t) + sizeof(uint64_t);
  /*
  struct Handshake {
    size_t threads_amt;
    uint64_t file_size;
    std::string file_nm;
  };
 */
  SendHandshakeBuff send_buff;
  std::copy(
      reinterpret_cast<void *>(&threads_amt_),
      reinterpret_cast<void *>(&threads_amt_ + threads_amt_off),
      reinterpret_cast<void *>(&send_buff.buffer)
  );
  auto file_size = static_cast<uint64_t>(get_file_size(fd));
  std::copy(
      reinterpret_cast<void *>(&file_size),
      reinterpret_cast<void *>(&file_size + sizeof(uint64_t)),
      reinterpret_cast<void *>(&send_buff.buffer + threads_amt_off)
  );
  char filename[FILENAME_MAX];
  get_file_name(file_path_.c_str(), filename, FILENAME_MAX);
  std::copy(
      reinterpret_cast<void *>(filename),
      reinterpret_cast<void *>(filename + FILENAME_MAX),
      reinterpret_cast<void *>(&send_buff + file_size_off)
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
  for (size_t port_idx = 0; port_idx < threads_amt_; ++port_idx) {
    ports.emplace_back(recv_buff.ports[port_idx]);
  }
}
