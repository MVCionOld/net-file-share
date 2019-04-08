#ifndef SRC_CSENDER_H
#define SRC_CSENDER_H

#include <cstddef>
#include <string>
#include <utility>

#include "../handshake/Handshake.hpp"

extern "C" {
#include "../handshake/glob_consts.h"
#if defined(_WIN32) || defined(_WIN64)
#include "win/SenderSocket.h"
#include "../file_mapping/win/FileMapping.h"
#else
#include "linux/sender_socket.h"
#include "../file_mapping/linux/file_mapping.h"
#endif
};

class CSender {
public:
  explicit CSender (std::string ip);

  void Send (std::string file_path, size_t threads_amt);

  ~CSender ();

private:
  void makeHandshake ();

private:
  int sockfd_ = ERROR_FD;
  size_t threads_amt_ = 1;
  std::string file_path_;
  std::string ip_;
};

#endif //SRC_CSENDER_H
