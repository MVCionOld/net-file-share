#ifndef SRC_RECEIVER_SOCKET_H
#define SRC_RECEIVER_SOCKET_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/unistd.h>

#include "../../handshake/glob_consts.h"

extern uint16_t
get_available_port ();

extern int
get_ready_sockrfd (uint16_t port);

extern int
accept_clientfd (int sockfd);

extern ssize_t
recv_package (int clientfd, char *data, size_t size);

extern void
close_sockrfd (int fd);

#endif //SRC_RECEIVER_SOCKET_H
