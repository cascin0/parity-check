#include "socket.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static void handle_error(char *error_message)
{
  perror(error_message);
  // Just quitting the program for simplicity
  exit(EXIT_FAILURE);
}

void create_server_socket(int port, Socket *server_socket)
{
  int file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (file_descriptor == -1)
    handle_error("Could not create the socket\n");
  else
    server_socket->file_descriptor = file_descriptor;

  struct sockaddr_in server_socket_address;
  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_addr.s_addr = INADDR_ANY; // All local interfaces
  server_socket_address.sin_port = htons(port);
  server_socket->address = server_socket_address;
}

void bind_server_socket(Socket *server_socket)
{
  int fd = server_socket->file_descriptor;
  struct sockaddr_in addr = server_socket->address;

  if (bind(fd, (const struct sockaddr *)(&addr), sizeof(addr)) == -1)
    handle_error("Could not bind the socket\n");
}

void server_socket_listen(Socket *server_socket, int backlog)
{
  if (listen(server_socket->file_descriptor, backlog) == -1)
    handle_error("Listen failed\n");
}

int socket_accept(Socket *socket, struct sockaddr *addr, socklen_t *addrlen)
{
  int fd = accept(socket->file_descriptor, addr, addrlen);

  if (fd == -1)
    handle_error("Accept failed\n");

  return fd;
}

int socket_read(int fd, char *buf, size_t count)
{
  int bytes_read = read(fd, buf, count);

  if (bytes_read == -1)
    handle_error("Read errror\n");

  return bytes_read;
}
