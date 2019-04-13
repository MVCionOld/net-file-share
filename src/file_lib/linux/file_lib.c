#include <unistd.h>
#include "file_lib.h"


int
create_file (const char *path) {
  const int flags = O_CREAT | O_RDWR | O_TRUNC
          #ifndef __APPLE__
                  | O_LARGEFILE
          #endif
                  ;
  return open(path, flags, OPEN_MODE);
}

int
open_file (const char *path) {
  const int flags = O_RDONLY
            #ifndef __APPLE__
                | O_LARGEFILE
            #endif
                ;
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
read_mmap (void *dst, void *src, size_t length, off_t offset) {
  memcpy(dst, src + offset, length);
}

void
write_mmap (void *dst, void *src, size_t length, off_t offset) {
  memcpy(dst + offset, src, length);
}

void
get_file_name (const char *path, char *filename, size_t length) {
  memset(filename, '\0', length);
  memcpy(filename, basename((char *) path), length);
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
map_file_r (int fd, off_t length) {
  return mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
}

void *
map_file_w (int fd, off_t length) {
  return mmap(NULL, length, PROT_WRITE, MAP_SHARED, fd, 0);
}

int
unmap_file (void *addr, off_t length) {
  return munmap(addr, length);
}