#include "file_mapping.h"


void *
map_file (int fd, size_t length, int prot, off_t offset) {
  if (prot == ALIAS_PROT_READ) {
    prot = PROT_READ;
  }
  else if (prot == ALIAS_PROT_WRITE) {
    prot = PROT_WRITE;
  }
  else {
    prot = PROT_NONE;
  }
  return mmap(NULL, length, prot, MAP_SHARED, fd, offset);
}

int
unmap_file (void *addr, size_t length) {
  return munmap(addr, length);
}