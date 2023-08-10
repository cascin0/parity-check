#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define SEND_BUFFER_SIZE 128

static inline int has_even_parity(uint8_t x) {
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;

  return (~x) & 1;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    perror("Server socket ip and port must be passed as arguments\n");
    exit(EXIT_FAILURE);
  } else if (argc > 3) {
    perror("Too many arguments\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_socket_addr;

  if (inet_aton(argv[1],
                (struct in_addr *)&server_socket_addr.sin_addr.s_addr) == 0) {
    perror("Invalid ip, insert one like 127.0.0.1\n");
    exit(EXIT_FAILURE);
  }

  int server_socket_port = atoi(argv[2]);

  // This branch (implicitly) exits even if the socket port is something
  // other than a number, since 'atoi' returns '0' for invalid arguments.
  // Checking for the specific error might be considererd, but the output
  // message seems good enough
  if (server_socket_port < 1024 || server_socket_port > 49151) {
    perror("Invalid socket port, insert one in the range 1024-49151\n");
    exit(EXIT_FAILURE);
  }

  int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket_fd == -1) {
    perror("Couldn't create the socket\n");
    exit(EXIT_FAILURE);
  }

  server_socket_addr.sin_family = AF_INET;
  server_socket_addr.sin_port = htons(server_socket_port);

  printf("Connecting to %s at port %d\n", argv[1], server_socket_port);

  if (connect(server_socket_fd, (const struct sockaddr *)&server_socket_addr,
              sizeof(server_socket_addr)) == -1) {
    perror("Connect failed\n");
    exit(EXIT_FAILURE);
  }

  // For demonstrative purposes, the client is going to send ordered
  // integers and randomly modify a variable number of bits of some of
  // them. The parity check is going to be the most significant bit

  char send_buf[SEND_BUFFER_SIZE] = {0};

  for (uint8_t i = 0; i < 128; i++) {
    send_buf[i] = i;

    int even_parity = has_even_parity(i);

    // Set most significant bit to 1
    if (!even_parity)
      send_buf[i] |= 128;
  }

  if (send(server_socket_fd, send_buf, SEND_BUFFER_SIZE, 0) == -1) {
    perror("Send failed\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
