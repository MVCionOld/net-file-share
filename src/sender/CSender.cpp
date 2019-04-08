#include "CSender.hpp"


CSender::CSender (std::string ip)
    : ip_(std::move(ip)) {}

CSender::~CSender () {
  close_socksfd(sockfd_);
}

void CSender::Send (std::string file_path, size_t threads_amt) {
  file_path_ = std::move(file_path);
  threads_amt_ = threads_amt;
  const int fd = open_file(file_path_.c_str());
  const uint16_t port = choose_port(
      ip_.c_str(),
      PortRange::FROM,
      PortRange::TO
  );
  std::vector<uint16_t> ports = {port};
  sockfd_ = get_ready_socksfd(ip_.c_str(), port);
  makeHandshake(fd, ports);
  std::vector<int> sockfds = {sockfd_};
  std::vector<std::thread> senders;
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    if (thread_id != 0) {
      const int socksfd = get_ready_socksfd(ip_.c_str(), ports[thread_id]);
      sockfds.emplace_back(socksfd);
    }
    senders.emplace_back(
        [thread_id] () {

          // TO_DO: add sender work

        });
  }
  for (auto &sender: senders) {
    sender.join();
  }
  for (auto socksfd: sockfds) {
    close_socksfd(socksfd);
  }
  close_file(fd);
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
