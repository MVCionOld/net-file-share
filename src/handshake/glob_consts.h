#ifndef SRC_GLOB_CONSTS_H
#define SRC_GLOB_CONSTS_H

#include <stdio.h>
#include <inttypes.h>


typedef char byte;

enum FdTypeVal {
  ERROR_FD = -1,
  OPEN_MODE = 0660
};

enum PortRange {
  FROM = 59000,
  TO = 60024
};

enum HandshakeVal {
  CONTROL_CODE_SIZE = 64,
  HANDSHAKE_SIZE = FILENAME_MAX + sizeof(uint64_t) + sizeof(size_t)
};

enum SendRecvParams {
  MAX_THREADS_AMT = 128,
  PACKAGE_SIZE = 4096
};

#endif //SRC_GLOB_CONSTS_H
