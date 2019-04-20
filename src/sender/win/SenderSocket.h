#ifndef SRC_SENDERSOCKET_H
#define SRC_SENDERSOCKET_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "../../handshake/glob_consts.h"


extern int
get_ready_socksfd(const char *ip, uint16_t port);

extern void
close_socksfd(int fd);

#endif //SRC_SENDERSOCKET_H
