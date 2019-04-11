#ifndef SRC_CFILEINFO_H
#define SRC_CFILEINFO_H

#include <glob.h>

#include "glob_consts.h"


struct SendHandshakeBuff {
  byte buffer[HANDSHAKE_SIZE] = {0};
};

struct RecvHandshakeBuff {
  size_t threads_amt = 0;
  size_t ports[MAX_THREADS_AMT] = {0};
};


extern void
parse (byte *dst, const byte *src, size_t size);

#endif //SRC_CFILEINFO_H
