#ifndef SRC_MEMORY_MAPPING_H
#define SRC_MEMORY_MAPPING_H

#include <dirent.h>
#include <limits.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "../../handshake/glob_consts.h"


extern void *
map_file (int fd, size_t length, int prot, off_t offset);

extern int
unmap_file (void *addr, size_t length);

#endif //SRC_MEMORY_MAPPING_H
