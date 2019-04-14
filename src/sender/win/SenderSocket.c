#include "SenderSocket.h"


int
get_ready_socksfd(const char *ip, uint16_t port) {
  SOCKET sockfd = INVALID_SOCKET;
  struct addrinfo *result = NULL;
  struct addrinfo *ptr = NULL,
  struct addrinfo	hints;
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
      &result
  );
  if (errcode != 0) {
    return ERROR_FD;
  }
  sockfd = socket(
      ptr->ai_family,
      ptr->ai_socktype,
      ptr->ai_protocol
  );
  if (sockfd == INVALID_SOCKET) {
    freeaddrinfo(result);
    return ERROR_FD;
  }
  errcode = connect(
      sockfd,
      ptr->ai_addr,
      (int)ptr->ai_addrlen
  );
  if (errcode == SOCKET_ERROR) {
    closesocket(sockfd);
    freeaddrinfo(result);
    return ERROR_FD;
  }
  return sockfd;
}

void
close_socksfd(int fd) {
  closesocket((SOCKET)fd);
}

