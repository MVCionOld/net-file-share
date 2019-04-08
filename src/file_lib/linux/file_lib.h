#ifndef SRC_MEMORY_MAPPING_H
#define SRC_MEMORY_MAPPING_H

#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64

#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include "../../handshake/glob_consts.h"


extern int
create_file (const char *path);

extern int
open_file (const char *path);

extern int
close_file (int fd);

extern ssize_t
read_package (int fd, void *buf, size_t count);

extern ssize_t
write_package (int fd, const void *buf, size_t count);

extern void
get_file_name (const char *path, char *filename, size_t length);

extern off64_t
get_file_size (int fd);

extern int
file_truncate (int fd, off_t length);

extern void *
map_file (int fd, size_t length, int prot, off_t offset);

extern int
unmap_file (void *addr, size_t length);

#endif //SRC_MEMORY_MAPPING_H
