#ifndef SRC_CFILEINFO_H
#define SRC_CFILEINFO_H

#include <string>

#include "glob_consts.h"


struct CHandshake {
  size_t threads_amt;
  uint64_t file_size;
  std::string file_nm;
};

struct CHandshakeBuff {
  char buffer[HandshakeVal::MAX_SIZE];
};


#endif //SRC_CFILEINFO_H
