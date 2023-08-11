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
    perror("Server's socket port must be passed as argument\n");
    exit(EXIT_FAILURE);
  }
  else if (argc > 2)
  {
    perror("Too many arguments\n");
    exit(EXIT_FAILURE);
  }

  int server_socket_port = atoi(argv[1]);

  // This branch (implicitly) exits even if the socket port is something
  // other than a number, since 'atoi' returns '0' for invalid arguments.
  // Checking for the specific error might be considererd, but the output
  // message seems good enough
  if (server_socket_port < 1024 || server_socket_port > 49151)
  {
    perror("Invalid socket port, insert one in the range 1024-49151\n");
    exit(EXIT_FAILURE);
  }

  Socket server_socket;
  socket_create(server_socket_port, SOCK_DGRAM, IPPROTO_UDP, &server_socket);

  socket_bind(&server_socket);

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  char read_buffer[READ_BUFFER_SIZE] = {0};

  int bytes_read = socket_recvfrom(server_socket.file_descriptor, read_buffer, READ_BUFFER_SIZE,
                                   (struct sockaddr *)&client_addr, &client_addr_len);

  printf("Accepted connection from %s at port %d\n",
         inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

  printf("Received %d bytes:\n", bytes_read);

  for (int i = 0; i < bytes_read; i++)
  {
    uint8_t raw_num = read_buffer[i]; // With the parity bit
    uint8_t num = raw_num & ~(128);   // Without the parity bit

    printf("received: %u, num: %u, ", raw_num, num);

    int even_parity = has_even_parity(raw_num);

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
