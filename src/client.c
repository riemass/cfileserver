#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "transfer_functions.h"
#include "request_type.h"
#include "init.h"

int main(int argc, char *argv[]) {
  int server;
  struct sockaddr_in server_addr;
  struct request req;
  size_t bufsize = 80;
  char *buffer = (char *)malloc(sizeof(char) * bufsize);
  char *token;

  init_client_addr(argc, argv, &server_addr);

  while (getline(&buffer, &bufsize, stdin) > 0) {
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
      perror("Could not create socket");
    }
    if (connect(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0) {
      perror("connect failed. Error");
      exit(1);
    }

    bufsize = send(server, buffer, strlen(buffer), 0);

    parse_request(buffer, &req);
    if (req.type == PUT) {
      send_file(server, req.file_path);
      printf("Connection succeded. Sending file: %s.\n", req.file_path);
    } else if (req.type == GET) {
      recv_file(server, req.file_path);
      printf("Connection succeded. Receiving file: %s.\n", req.file_path);
    } else {
      perror("Error: unrecognized method.");
    }
    close(server);
  }

  free(buffer);
  return 0;
}
