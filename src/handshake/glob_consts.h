#ifndef SRC_GLOB_CONSTS_H
#define SRC_GLOB_CONSTS_H

#include <stdio.h>

using mmap_t = void *;
using byte = char;

enum FdTypeVal {
  ERROR_FD = -1,
  OPEN_MODE = 0660
};

enum PortRange {
  FROM,
  TO
};

enum HandshakeVal {
  HANDSHAKE_SIZE = FILENAME_MAX + sizeof(uint64_t) + sizeof(size_t)
};

enum SendRecvParams {
  MAX_THREADS_AMT = 8,
  PACKAGE_SIZE = 1024
};

#endif //SRC_GLOB_CONSTS_H
