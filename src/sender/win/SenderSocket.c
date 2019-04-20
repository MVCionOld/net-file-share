#include "SenderSocket.h"


int
get_ready_socksfd (const char *ip, uint16_t port) {
  SOCKET sockfd;
  struct addrinfo *addrs = NULL;
  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  char port_buf[16];
  ZeroMemory(&port_buf, sizeof(port_buf));
  sprintf(port_buf, "%hu", port);
  int errcode = getaddrinfo(
      ip,
      port_buf,
      &hints,
      &addrs
  );
  if (errcode != 0) {
    return ERROR_FD;
  }
  sockfd = socket(
      addrs->ai_family,
      addrs->ai_socktype,
      addrs->ai_protocol
  );
  if (sockfd == INVALID_SOCKET) {
    freeaddrinfo(addrs);
    return ERROR_FD;
  }
  errcode = connect(
      sockfd,
      addrs->ai_addr,
      (int) addrs->ai_addrlen
  );
  if (errcode == SOCKET_ERROR) {
    closesocket(sockfd);
    freeaddrinfo(addrs);
    return ERROR_FD;
  }
  freeaddrinfo(addrs);
  return sockfd;
}

void
close_socksfd (int fd) {
  closesocket((SOCKET) fd);
}

