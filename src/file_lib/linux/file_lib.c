#include "file_lib.h"


int
get_file_desc (char *path, int flags);

const mode_t mode = 0640;
return

open ();
}

void
get_file_name (char *path, char *filename, size_t length) {
  memset(filename, '\0', length);
  memcpy(filename, basename(path), length);
}

off_t
get_file_size (int fd) {
  struct stat file_stat;
  if (fstat(fd, &file_stat) != -1) {
    return file_stat.st_size;
  }
  return 0;
}

int
file_truncate (int fd, off_t length) {
  return ftruncate(fd, length);
}

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