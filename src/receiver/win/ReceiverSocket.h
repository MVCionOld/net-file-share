#ifndef SRC_RECEIVERSOCKET_H
#define SRC_RECEIVERSOCKET_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>


extern uint16_t
choose_available_port(uint16_t from, uint16_t to);

extern int
get_ready_sockrfd(uint16_t port);

extern int
accept_clientfd(int sockfd);

extern void
close_sockrfd(int fd);

#endif //SRC_RECEIVERSOCKET_H
