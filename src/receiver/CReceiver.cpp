
#include "CReceiver.hpp"


CReceiver::CReceiver () {
  auto port = choose_available_port(PortRange::FROM, PortRange::TO);
  sockfd_ = get_ready_sockrfd(port);
}

CReceiver::~CReceiver () {
  close_sockrfd(sockfd_);
}

void CReceiver::Receive () {
  int clifd = accept_clientfd(sockfd_);
  std::vector<uint16_t> ports;
  std::string file_nm;
  uint64_t file_size;
  makeHandshake(clifd, ports, file_nm, file_size);
  int fd = prepareFile(file_nm, file_size);
  std::vector<int> sockfds = {sockfd_}, clifds = {clifd};
  std::vector<std::thread> receivers;
  for (size_t thread_id = 0; thread_id < threads_amt_; ++thread_id) {
    if (thread_id != 0) {
      sockfds.emplace_back(get_ready_sockrfd(ports[thread_id]));
      clifds.emplace_back(accept_clientfd(sockfds[thread_id]));
    }
    receivers.emplace_back(
        [thread_id] () {

          // TO_DO: add receiver work

        });
  }
  for (auto &receiver: receivers) {
    receiver.join();
  }
  for (auto clientfd: clifds) {
    close_sockrfd(clientfd);
  }
  for (auto sockfd: sockfds) {
    close_sockrfd(sockfd);
  }
  close_file(fd);
}


void CReceiver::makeHandshake (int clifd, std::vector<uint16_t> &ports,
                               std::string &file_nm, uint64_t &file_size) {
  SendHandshakeBuff buff;
  read_package(
      clifd,
      reinterpret_cast<void *>(&buff),
      HandshakeVal::HANDSHAKE_SIZE
  );
  /*
    struct Handshake {
      size_t threads_amt;
      uint64_t file_size;
      std::string file_nm;
    };
   */
  const size_t threads_amt_off = sizeof(size_t);
  const size_t file_size_off = sizeof(size_t) + sizeof(uint64_t);
  std::copy(
      reinterpret_cast<void *>(&buff),
      reinterpret_cast<void *>(&buff + threads_amt_off),
      reinterpret_cast<void *>(&threads_amt_)
  );
  std::copy(
      reinterpret_cast<void *>(&buff + threads_amt_off),
      reinterpret_cast<void *>(&buff + file_size_off),
      reinterpret_cast<void *>(&file_size)
  );
  file_nm = std::move(std::string(buff.buffer + file_size_off));
  RecvHandshakeBuff recv_buff = {
      .threads_amt = threads_amt_,
      .ports[0] = static_cast<size_t >(sockfd_)
  };
  ports.emplace_back(recv_buff.ports[0]);
  for (size_t port_idx = 1; port_idx < threads_amt_; ++port_idx) {
    recv_buff.ports[port_idx] = static_cast<size_t>(
        get_ready_sockrfd(choose_available_port(PortRange::FROM, PortRange::TO))
    );
    ports.emplace_back(recv_buff.ports[port_idx]);
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
