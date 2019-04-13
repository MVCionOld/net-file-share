#include "receiver_socket.h"


enum {
  MESSAGE_QUEUE_SIZE = 16
};


uint16_t
choose_available_port (uint16_t from, uint16_t to) {
  for (uint16_t port = from; port < to; ++port) {
    const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
      perror("socket");
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
    puts("Opened free port");
#endif
    const int errcode = bind(
        sockfd,
        (struct sockaddr *) &server_socket,
        sizeof(struct sockaddr_in)
    );
    if (errcode < 0) {
      continue;
    }
    close(sockfd);
    return port;
  }
  return 0;
}

int
get_ready_sockrfd (uint16_t port) {
  const int domain = AF_INET;
  const int sockfd = socket(domain, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    return ERROR_FD;
  }
  int optval = 1;
  const socklen_t optlen = (socklen_t) sizeof(optval);
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
  puts("Opened port");
#endif
  const int errcode = bind(
      sockfd,
      (struct sockaddr *) &server_socket,
      sizeof(struct sockaddr_in)
  );
  if (errcode < 0) {
    perror("bind");
    return ERROR_FD;
  }
  listen(sockfd, MESSAGE_QUEUE_SIZE);
  return sockfd;
}

int
accept_clientfd (int sockfd) {
  struct sockaddr_in client_sock;
  socklen_t socklen = sizeof(client_sock);
  const int clientfd = accept(
      sockfd,
      (struct sockaddr *) &client_sock,
      &socklen
  );
  return clientfd;
}

void
close_sockrfd (int fd) {
  close(fd);
}
