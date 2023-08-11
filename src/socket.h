#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

typedef struct
{
  int file_descriptor;
  struct sockaddr_in address;
} Socket;

static void handle_error(char *error_message);

void create_server_socket(int port, Socket *server_socket);
void bind_server_socket(Socket *server_socket);
void server_socket_listen(Socket *server_socket, int backlog);

int socket_read(int fd, char *buf, size_t count);

#endif
