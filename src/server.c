#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "transfer_functions.h"
#include "request_type.h"

#define BACKLOG 3

int main(int argc, char *argv[]) {
  int server;
  int client;
  int c;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  int opt;
  char buffer[80];
  struct request req;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);

  while ((opt = getopt(argc, argv, "s:")) != -1) {
    server_addr.sin_port = htons(strtoul(optarg, NULL, 10));
  }

  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0) {
    perror("Could not create socket. Error");
  }

  if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Bind failed. Error");
    exit(1);
  }
  listen(server, BACKLOG);

  c = sizeof(struct sockaddr_in);
  while (client = accept(server, (struct sockaddr *)&client_addr, (socklen_t *)&c)) {
  if (client < 0) {
    perror("Accept failed. Error");
    exit(1);
  }
  puts("Connection accepted. Writing incoming file.");

  read_request(client, buffer);
  parse_request(buffer, &req);
  if (req.type == PUT) {
    recv_file(client, req.file_path);
  } else if (req.type == GET) {
    send_file(client, req.file_path);
  }

  close(client);
  }

  close(server);

  return 0;
}
