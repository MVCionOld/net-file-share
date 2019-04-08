#ifndef SRC_CFILEINFO_H
#define SRC_CFILEINFO_H

#include <string>

extern "C" {
#include "glob_consts.h"
#if defined(_WIN32) || defined(_WIN64)
#include "../file_lib/win/FileLib.h"
#else
#include "../file_lib/linux/file_lib.h"
#endif
};


struct SendHandshakeBuff {
  char buffer[HandshakeVal::HANDSHAKE_SIZE] = {0};
};

struct RecvHandshakeBuff {
  size_t threads_amt = 0;
  size_t ports[HandshakeVal::MAX_THREADS_AMT] = {0};
};

#endif //SRC_CFILEINFO_H
