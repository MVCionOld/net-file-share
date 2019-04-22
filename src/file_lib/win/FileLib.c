#include "FileLib.h"


int
create_file (const char *path) {
  HANDLE file_handle = CreateFileA(
      path,
      GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL,
      NULL
  );
  if (file_handle == INVALID_HANDLE_VALUE) {
    return ERROR_FD;
  }
  int fd = _open_osfhandle(
      (intptr_t) file_handle,
      _O_APPEND | _O_RDONLY | _O_WRONLY | _O_TRUNC
  );
  if (fd == ERROR_FD) {
    CloseHandle(file_handle);
  }
  return fd;
}

int
open_file (const char *path) {
  HANDLE file_handle = CreateFileA(
      path,                                         // file to open
      GENERIC_READ,                                 // open for reading
      FILE_SHARE_READ,                              // share for reading
      NULL,                                         // default security
      OPEN_EXISTING,                                // existing file only
      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // normal file
      NULL                                          // no attr. template
  );
  if (file_handle == INVALID_HANDLE_VALUE) {
    return ERROR_FD;
  }
  int fd = _open_osfhandle(
      (intptr_t) file_handle,
      _O_RDONLY
  );
  if (fd == ERROR_FD) {
    CloseHandle(file_handle);
  }
  return fd;
}

int
close_file (int fd) {
  return _close(fd);  // Also calls CloseHandle()
}

int
read_package (int fd, void *buf, uint32_t count) {
  return recv((SOCKET) fd, (char *) buf, count, 0);
}

int
write_package (int fd, const void *buf, uint32_t count) {
  return send((SOCKET) fd, (const char *) buf, count, 0);
}

void
read_mmap (void *dst, void *src, uint32_t length, uint64_t offset) {
  memcpy(dst, (char *) src + offset, length);
}

void
write_mmap (void *dst, void *src, uint32_t length, uint64_t offset) {
  memcpy((char *) dst + offset, src, length);
}

void
get_file_name (const char *path, char *filename, uint32_t length) {
  memcpy(filename, path, length);
  PathStripPathA((TCHAR *) filename);
}

uint64_t
get_file_size (int fd) {
  return _filelengthi64(fd);
}

int
file_truncate (int fd, uint64_t length) {
  return _chsize_s(fd, length);
}

void *
map_file_r (int fd, uint64_t length) {
  HANDLE file_handle = (HANDLE) _get_osfhandle(fd);
  if (file_handle == INVALID_HANDLE_VALUE) {
    return NULL;
  }
  HANDLE map_handle = CreateFileMapping(
      file_handle,
      NULL,
      PAGE_READONLY,
      0,
      0,
      NULL
  );
  if (map_handle == NULL) {
    return NULL;
  }
  return MapViewOfFile(
      map_handle,
      FILE_MAP_READ,
      0,
      0,
      length
  );
}

void *
map_file_w (int fd, uint64_t length) {
  HANDLE file_handle = (HANDLE) _get_osfhandle(fd);
  if (file_handle == INVALID_HANDLE_VALUE) {
    return NULL;
  }
  HANDLE map_handle = CreateFileMapping(
      file_handle,
      NULL,
      PAGE_READWRITE,
      0,
      0,
      NULL
  );
  if (map_handle == NULL) {
    return NULL;
  }
  return MapViewOfFile(
      map_handle,
      FILE_MAP_WRITE,
      0,
      0,
      length
  );
}

int
unmap_file (void *addr, uint64_t length) {
  return UnmapViewOfFile(addr);
}
