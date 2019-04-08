#ifndef SRC_GLOB_CONSTS_H
#define SRC_GLOB_CONSTS_H

#include <stdio.h>

enum FdTypeVal {
  ERROR_FD = -1
};

enum HandshakeVal {
  MAX_SIZE = FILENAME_MAX + sizeof(uint64_t) + sizeof(size_t)
};

enum ProtVal {
  ALIAS_PROT_NONE = -1,
  ALIAS_PROT_READ = 0,
  ALIAS_PROT_WRITE = 1
};

enum FileFlags {

};

#endif //SRC_GLOB_CONSTS_H
