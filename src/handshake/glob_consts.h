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
  FROM = 59060,
  TO = 60084
};

enum HandshakeVal {
  CONTROL_CODE_SIZE = 64,
  HANDSHAKE_SIZE = FILENAME_MAX + sizeof(uint64_t) + sizeof(size_t)
};

enum SendRecvParams {
  MAX_THREADS_AMT = 12,
  PACKAGE_SIZE = 1024
};

#endif //SRC_GLOB_CONSTS_H
