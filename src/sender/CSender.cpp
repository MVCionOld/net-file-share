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
  auto source_map = map_file_r(source_fd, file_size);
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
  const size_t file_size_off = sizeof(size_t);
  const size_t file_nm_off = sizeof(size_t) + sizeof(uint64_t);
  /*
    Handshake
    {
      size_t   threads_amt
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
  auto file_size = static_cast<uint64_t>(get_file_size(fd));
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
  for (size_t port_idx = 0; port_idx < threads_amt_; ++port_idx) {
    ports.emplace_back(recv_buff.ports[port_idx]);
  }
}
