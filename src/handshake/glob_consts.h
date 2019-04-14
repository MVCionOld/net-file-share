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
  FILENAME_MAX_= 4096,
  HANDSHAKE_SIZE = FILENAME_MAX_ + sizeof(uint64_t) + sizeof(size_t)
};

enum SendRecvParams {
  MAX_THREADS_AMT = 128,
  PACKAGE_SIZE = 4096
};

enum ProgressBarVal {
  BUFFER_SIZE = 256
};

#endif //SRC_GLOB_CONSTS_H
