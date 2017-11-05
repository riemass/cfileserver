#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "transfer_functions.h"

int main(int argc, char *argv[]) {
  int server;
  struct sockaddr_in server_addr;
  char *file_path;
  int opt;

  if (argc < 2) {
    puts("Usage:\n\tsendfile [-a ip -s socket] path");
    exit(1);
  }
  file_path = argv[argc - 1];

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);

  while ((opt = getopt(argc, argv, "s:a:")) != -1) {
    switch(opt) {
      case 's':
        server_addr.sin_port = htons(strtoul(optarg, NULL, 10));
        break;
      case 'a':
        server_addr.sin_addr.s_addr = inet_addr(optarg);
        break;
    }
  }

  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0) {
    perror("Could not create socket");
  }

  if (connect(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("connect failed. Error");
    exit(1);
  }
  puts("Connected. Sending file...");

  send_file(server, file_path);

  close(server);
  return 0;
}
