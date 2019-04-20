#include "handshake.hpp"


void
parse (char *dst, const char *src, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    *(dst + i) = *(src + i);
  }
}
