#ifndef SRC_SENDER_SOCKET_H
#define SRC_SENDER_SOCKET_H

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "../../handshake/glob_consts.h"

extern uint16_t
choose_port (const char *ip, uint16_t from, uint16_t to);

extern int
get_ready_socksfd (const char *ip, uint16_t port);

extern void
close_socksfd (int fd);

#endif //SRC_SENDER_SOCKET_H
