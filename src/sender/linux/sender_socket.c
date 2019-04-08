#include "sender_socket.h"


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

ssize_t
send_package (int sockfd, char *data, size_t size) {
  return write(sockfd, data, size);
}

void
close_socksfd (int fd) {
  close(fd);
}