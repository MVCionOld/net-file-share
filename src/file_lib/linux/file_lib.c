#include "file_lib.h"


int
create_file (const char *path) {
  const int flags = O_CREAT | O_WRONLY | O_LARGEFILE | O_NONBLOCK;
  return open(path, flags, OPEN_MODE);
}

int
open_file (const char *path) {
  const int flags = O_RDONLY | O_LARGEFILE | O_NONBLOCK;
  return open(path, flags, OPEN_MODE);
}

int
close_file (int fd) {
  return close(fd);
}

ssize_t
read_package (int fd, void *buf, size_t count) {
  return read(fd, buf, count);
}

ssize_t
write_package (int fd, const void *buf, size_t count) {
  return write(fd, buf, count);
}

void
get_file_name (const char *path, char *filename, size_t length) {
  memset(filename, '\0', length);
  memcpy(filename, basename((char *) path), length);
}

off64_t
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
  } else if (prot == ALIAS_PROT_WRITE) {
    prot = PROT_WRITE;
  } else {
    prot = PROT_NONE;
  }
  return mmap(NULL, length, prot, MAP_SHARED, fd, offset);
}

int
unmap_file (void *addr, size_t length) {
  return munmap(addr, length);
}