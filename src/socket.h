#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

typedef struct
{
  int file_descriptor;
  struct sockaddr_in address;
} Socket;

static void handle_error(char *error_message);

void socket_create(int port, int type, int flags, Socket *sock);
void socket_bind(Socket *sock);

int socket_recvfrom(int fd, char *buf, size_t buf_len, struct sockaddr *addr, socklen_t *addr_len);
int socket_sendto(int fd, char *buf, size_t buf_len, struct sockaddr *addr, socklen_t addr_len);

#endif
