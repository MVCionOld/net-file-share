#ifndef SRC_GLOB_CONSTS_H
#define SRC_GLOB_CONSTS_H

#include <stdio.h>

enum FdTypeVal {
  ERROR_FD = -1,
  OPEN_MODE = 0640
};

enum PortRange {
  FROM,
  TO
};

enum HandshakeVal {
  HANDSHAKE_SIZE = FILENAME_MAX + sizeof(uint64_t) + sizeof(size_t),
  MAX_THREADS_AMT = 8
};

enum ProtVal {
  ALIAS_PROT_NONE,
  ALIAS_PROT_READ,
  ALIAS_PROT_WRITE
};

#endif //SRC_GLOB_CONSTS_H
