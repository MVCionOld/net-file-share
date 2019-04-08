#ifndef SRC_CSENDER_H
#define SRC_CSENDER_H

#include <cstddef>
#include <cstring>
#include <thread>
#include <utility>
#include <vector>

#include "../handshake/Handshake.hpp"

extern "C" {
#include "../handshake/glob_consts.h"
#if defined(_WIN32) || defined(_WIN64)
#include "win/SenderSocket.h"
#include "../file_lib/win/FileLib.h"
#else
#include "linux/sender_socket.h"
#include "../file_lib/linux/file_lib.h"
#endif
};


class CSender {
public:
  explicit CSender (std::string ip);

  void Send (std::string file_path, size_t threads_amt);

  ~CSender ();

private:
  void makeHandshake (int fd, std::vector<uint16_t> &ports);

private:
  int sockfd_ = ERROR_FD;
  size_t threads_amt_ = 1;
  std::string file_path_;
  std::string ip_;
};

#endif //SRC_CSENDER_H
