#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

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

  return 0;
}
