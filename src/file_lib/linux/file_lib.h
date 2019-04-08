#ifndef SRC_MEMORY_MAPPING_H
#define SRC_MEMORY_MAPPING_H

#define _GNU_SOURCE

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
get_file_desc (char *path, int flags, mode_t mode);

extern void
get_file_name (char *path, char *filename, size_t length);

extern off_t
get_file_size (char *filename);

extern int
file_truncate (int fd, off_t length);

extern void *
map_file (int fd, size_t length, int prot, off_t offset);

extern int
unmap_file (void *addr, size_t length);

#endif //SRC_MEMORY_MAPPING_H
