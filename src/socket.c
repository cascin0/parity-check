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

void socket_create(int port, int type, int flags, Socket *sock)
{
  int file_descriptor = socket(AF_INET, type, flags);
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

int socket_recvfrom(int fd, char *buf, size_t buf_len, struct sockaddr *addr, socklen_t *addr_len)
{
  int bytes_read = recvfrom(fd, buf, buf_len, 0, addr, addr_len);

  if (bytes_read == -1)
    handle_error("Recvfrom failed\n");

  return bytes_read;
}

int socket_sendto(int fd, char *buf, size_t buf_len, struct sockaddr *addr, socklen_t addr_len)
{
  int bytes_sent = sendto(fd, buf, buf_len, 0, addr, addr_len);

  if (bytes_sent == -1)
    handle_error("Sendto failed\n");

  return bytes_sent;
}
