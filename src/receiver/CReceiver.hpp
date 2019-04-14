#ifndef SRC_CRECEIVER_H
#define SRC_CRECEIVER_H

#include <mutex>
#include <string>
#include <thread>
#include <vector>


#include "../handshake/handshake.hpp"
#include "../utils/ProgressBar.hpp"

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
  int makeHandshake ();

  void setUpConnection (int clifd, std::vector<int> &sockfds,
                        std::vector<int> &clifds,
                        std::string &file_nm, uint64_t &file_size,
                        std::vector<std::thread> &accepters);

  int prepareFile (const std::string &file_nm, uint64_t file_size);

private:
  int sockfd_ = ERROR_FD;
  size_t threads_amt_ = 1, activated_port_ = 1;
  std::mutex ports_mutex_;
  std::atomic<size_t> packages_received_{0};
};

#endif //SRC_CRECEIVER_H
