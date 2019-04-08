#include "sender_socket.h"


uint16_t
choose_port (const char *ip, uint16_t from, uint16_t to) {
  /*
   *
   * TO-DO: find available receiver's port
   *
   */
  return 27650;
}

int
get_ready_socksfd (const char *ip, uint16_t port) {
#ifdef DEBUG
  const int domain = PF_INET;
#else
  const int domain = AF_INET;
#endif
  char service[8] = {'\0'};
  sprintf(service, "%hu", port);
  struct addrinfo *addrs = NULL;
  int errcode = getaddrinfo(
      ip,
      service,
      &(struct addrinfo) {
          .ai_family = domain,
          .ai_socktype = SOCK_STREAM
      },
      &addrs
  );
  if (errcode) {
    perror("getaddrinfo");
    return ERROR_FD;
  }
  int sockfd = socket(domain, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    freeaddrinfo(addrs);
    return ERROR_FD;
  }
  errcode = connect(
      sockfd,
      addrs->ai_addr,
      sizeof(struct sockaddr_in)
  );
  if (errcode < 0) {
    perror("connect");
    freeaddrinfo(addrs);
    return ERROR_FD;
  }
  freeaddrinfo(addrs);
  return sockfd;
}

void
close_socksfd (int fd) {
  close(fd);
}