#include "parity.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "socket.h"

#define SEND_BUFFER_SIZE 256

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    perror("Server socket ip and port must be passed as arguments\n");
    exit(EXIT_FAILURE);
  }
  else if (argc > 3)
  {
    perror("Too many arguments\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_socket_addr;

  if (inet_aton(argv[1],
                (struct in_addr *)&server_socket_addr.sin_addr.s_addr) == 0)
  {
    perror("Invalid ip, insert one like 127.0.0.1\n");
    exit(EXIT_FAILURE);
  }

  int server_socket_port = atoi(argv[2]);

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
  socket_create(server_socket_port, &server_socket);

  printf("Connecting to %s at port %d\n", argv[1], server_socket_port);

  socket_connect(&server_socket);

  char send_buf[SEND_BUFFER_SIZE] = {0};

  // Generate OK numbers
  for (uint8_t i = 0; i < 128; i++)
  {
    send_buf[i] = i | (has_even_parity(i) * 128);
    // Totally arbitrary bit alteration. Note that some of these
    // might not actually do anything
    send_buf[i + 128] = send_buf[i] | 4;
  }

  socket_send(server_socket.file_descriptor, send_buf, SEND_BUFFER_SIZE);

  return 0;
}
