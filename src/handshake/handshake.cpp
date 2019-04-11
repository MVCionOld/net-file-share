#include "handshake.hpp"


void
parse (byte *dst, const byte *src, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    *(dst + i) = *(src + i);
  }
}
