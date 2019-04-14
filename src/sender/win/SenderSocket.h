#ifndef SRC_SENDERSOCKET_H
#define SRC_SENDERSOCKET_H

#include <inttypes.h>


extern int
get_ready_socksfd (const char *ip, uint16_t port);

extern void
close_socksfd (int fd);

#endif //SRC_SENDERSOCKET_H
