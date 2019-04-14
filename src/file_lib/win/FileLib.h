#ifndef SRC_MEMORYMAPPING_H
#define SRC_MEMORYMAPPING_H

#include <fileapi.h>
#include <io.h>
#include <memryapi.h>
#include <shlwapi.h>
#include <stdint.h>
#include <zconf.h>

#include "../../handshake/glob_consts.h"

extern int
create_file (const char *path);

extern int
open_file (const char *path);

extern int
close_file (int fd);

extern int
read_package (int fd, void *buf, uint32_t count);

extern int
write_package (int fd, const void *buf, uint32_t count);

extern void
read_mmap (void *dst, void *src, uint32_t length, uint64_t offset);

extern void
write_mmap (void *dst, void *src, uint32_t length, uint64_t offset);

extern void
get_file_name (const char *path, char *filename, uint32_t length);

extern uint64_t
get_file_size (int fd);

extern int
file_truncate (int fd, uint64_t length);

extern void *
map_file_r (int fd, uint64_t length);

extern void *
map_file_w (int fd, uint64_t length);

extern int
unmap_file (void *addr, uint64_t length);

#endif //SRC_MEMORYMAPPING_H
