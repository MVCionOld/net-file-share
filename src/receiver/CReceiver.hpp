#ifndef SRC_CRECEIVER_H
#define SRC_CRECEIVER_H

#include <string>
#include <thread>
#include <vector>


#include "../handshake/handshake.hpp"
extern "C" {
#include "../handshake/glob_consts.h"
#if defined(_WIN32) || defined(_WIN64)
#include "win/ReceiverSocket.h"
#include "../file_lib/win/FileLib.h"
#else
#include "linux/receiver_socket.h"
#include "../file_lib/linux/file_lib.h"
#endif
};


class CReceiver {
public:
  explicit CReceiver ();

  void Receive ();

  ~CReceiver ();

private:
  void makeHandshake (int clifd, std::vector<uint16_t> &ports,
                      std::string &file_nm, uint64_t &file_size);

  int prepareFile (const std::string &file_nm, uint64_t file_size);

private:
  int sockfd_ = ERROR_FD;
  size_t threads_amt_ = 1, port_;
};

#endif //SRC_CRECEIVER_H
