#ifndef SRC_CRECEIVER_H
#define SRC_CRECEIVER_H

#include <string>

#include "../handshake/Handshake.hpp"

extern "C" {
#include "../handshake/glob_consts.h"
#if defined(_WIN32) || defined(_WIN64)
#include "win/ReceiverSocket.h"
#include "../file_mapping/win/FileMapping.h"
#else
#include "linux/receiver_socket.h"
#include "../file_mapping/linux/file_mapping.h"
#endif
};

class CReceiver {
public:
  explicit CReceiver ();

  void Receive () const;

  ~CReceiver ();

private:
  void makeHandshake ();

private:
  int sockfd_ = ERROR_FD;
  size_t threads_amt = 1;
  std::string file_path_;
};

#endif //SRC_CRECEIVER_H
