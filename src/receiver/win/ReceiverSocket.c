#include "ReceiverSocket.h"


uint16_t
choose_available_port (uint16_t from, uint16_t to) {
  for (uint16_t port = from; port < to; ++port) {
    const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      return 0;
    }
    int optval = 1;
    const socklen_t optlen = (socklen_t)
        sizeof(optval);
    setsockopt(
        sockfd,
        SOL_SOCKET,
        SO_REUSEADDR,
        &optval,
        optlen
    );
    setsockopt(
        sockfd,
        SOL_SOCKET,
        SO_REUSEPORT,
        &optval,
        optlen
    );
    struct sockaddr_in server_socket = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };
#ifdef DEBUG
    puts("Opened possible free port.");
#endif
    const int errcode = bind(
        sockfd,
        (struct sockaddr *) &server_socket,
        sizeof(struct sockaddr_in)
    );
    if (errcode < 0) {
      continue;
    }
    closesocket(sockfd);
    return port;
  }
  return 0;
}

int
get_ready_sockrfd (uint16_t port) {
  SOCKET sockfd = INVALID_SOCKET;
  struct addrinfo *result = NULL;
  struct addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  char port_buf[16];
  ZeroMemory(&port_buf, sizeof(port_buf));
  sprintf(port_buf, "%hu", port);
  int errcode = getaddrinfo(
      NULL,
      port_buf,
      &hints,
      &result
  );
  if (errcode != 0) {
    return ERROR_FD;
  }
  sockfd = socket(
      result->ai_family,
      result->ai_socktype,
      result->ai_protocol
  );
  if (sockfd == INVALID_SOCKET) {
    freeaddrinfo(result);
    return ERROR_FD;
  }
  int optval = 1;
  const socklen_t optlen = (socklen_t)
  sizeof(optval);
  setsockopt(
      sockfd,
      SOL_SOCKET,
      SO_REUSEADDR,
      &optval,
      optlen
  );
  setsockopt(
      sockfd,
      SOL_SOCKET,
      SO_REUSEPORT,
      &optval,
      optlen
  );
  errcode = bind(
      sockfd,
      result->ai_addr,
      (int) result->ai_addrlen
  );
  if (errcode == SOCKET_ERROR) {
    freeaddrinfo(result);
    closesocket(sockfd);
    return ERROR_FD;
  }
  freeaddrinfo(result);
  errcode = listen(sockfd, SOMAXCONN);
  if (errcode == SOCKET_ERROR) {
    closesocket(sockfd);
    return ERROR_FD;
  }
  return sockfd;
}

int
accept_clientfd (int sockfd) {
  struct sockaddr_in client_sock;
  socklen_t socklen = sizeof(client_sock);
  SOCKET clifd = accept(
      (SOCKET) sockfd,
      (struct sockaddr *) &client_sock,
      &socklen
  );
  if (clifd == INVALID_SOCKET) {
    return ERROR_FD;
  }
  return (int) clifd;
}

void
close_sockrfd (int fd) {
  closesocket((SOCKET) fd);
}
