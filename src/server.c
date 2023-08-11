#include "parity.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

#define READ_BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    perror("Socket port must be passed as argument\n");
    exit(EXIT_FAILURE);
  }
  else if (argc > 2)
  {
    perror("Too many arguments\n");
    exit(EXIT_FAILURE);
  }

  int socket_port = atoi(argv[1]);

  // This branch (implicitly) exits even if the socket port is something
  // other than a number, since 'atoi' returns '0' for invalid arguments.
  // Checking for the specific error might be considererd, but the output
  // message seems good enough
  if (socket_port < 1024 || socket_port > 49151)
  {
    perror("Invalid socket port, insert one in the range 1024-49151\n");
    exit(EXIT_FAILURE);
  }

  Socket server_socket;
  create_server_socket(socket_port, &server_socket);

  bind_server_socket(&server_socket);

  // For simplicity, the server only accepts a single incoming connection
  server_socket_listen(&server_socket, 1);

  printf("Listening at port %d\n", ntohs(server_socket.address.sin_port));

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  int client_fd = socket_accept(&server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

  printf("Accepted connection from %s at port %d\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  char read_buf[READ_BUFFER_SIZE] = {0};
  int bytes_read = socket_read(client_fd, read_buf, READ_BUFFER_SIZE);

  printf("Received %d bytes:\n", bytes_read);
  for (int i = 0; i < bytes_read; i++)
  {
    uint8_t x = read_buf[i];  // With the parity bit
    uint8_t num = x & ~(128); // Without the parity bit

    printf("received: %u, num: %u, ", x, num);

    int even_parity = has_even_parity(x);

    if (!even_parity)
    {
      printf("OK odd parity\n");
    }
    else
    {
      printf("ERROR even parity\n");
    }
  }

  if (close(server_socket.file_descriptor) == -1)
  {
    perror("Couldn't close the socket\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
