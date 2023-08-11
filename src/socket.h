#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

typedef struct
{
  int file_descriptor;
  struct sockaddr_in address;
} Socket;

static void handle_error(char *error_message);

void socket_create(int port, Socket *sock);

void socket_bind(Socket *sock);
void socket_listen(Socket *sock, int backlog);
void socket_connect(Socket *sock);

int socket_accept(Socket *sock, struct sockaddr *addr, socklen_t *addrlen);
int socket_read(int fd, char *buf, size_t count);
void socket_send(int fd, char *buf, size_t count);

#endif
