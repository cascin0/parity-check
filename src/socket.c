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

void socket_create(int port, Socket *sock)
{
  int file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (file_descriptor == -1)
    handle_error("Could not create the socket\n");
  else
    sock->file_descriptor = file_descriptor;

  struct sockaddr_in server_socket_address;
  server_socket_address.sin_family = AF_INET;
  server_socket_address.sin_addr.s_addr = INADDR_ANY; // All local interfaces
  server_socket_address.sin_port = htons(port);
  sock->address = server_socket_address;
}

void socket_bind(Socket *sock)
{
  int fd = sock->file_descriptor;
  struct sockaddr_in addr = sock->address;

  if (bind(fd, (const struct sockaddr *)(&addr), sizeof(addr)) == -1)
    handle_error("Could not bind the socket\n");
}

void socket_listen(Socket *sock, int backlog)
{
  if (listen(sock->file_descriptor, backlog) == -1)
    handle_error("Listen failed\n");
}

void socket_connect(Socket *sock)
{
  int fd = sock->file_descriptor;
  struct sockaddr_in addr = sock->address;

  if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
    handle_error("Connect failed\n");
}

int socket_accept(Socket *sock, struct sockaddr *addr, socklen_t *addrlen)
{
  int fd = accept(sock->file_descriptor, addr, addrlen);

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

void socket_send(int fd, char *buf, size_t count)
{
  if (send(fd, buf, count, 0) == -1)
    handle_error("Send failed\n");
}
