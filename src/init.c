#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "init.h"

void init_client_addr(int argc, char *argv[], struct sockaddr_in* server_addr) {
  int opt;

  memset(server_addr, 0, sizeof(*server_addr));
  server_addr->sin_addr.s_addr = INADDR_ANY;
  server_addr->sin_family = AF_INET;
  server_addr->sin_port = htons(8080);

  while ((opt = getopt(argc, argv, "s:a:")) != -1) {
    switch (opt) {
      case 's':
        server_addr->sin_port = htons(strtoul(optarg, NULL, 10));
        break;
      case 'a':
        server_addr->sin_addr.s_addr = inet_addr(optarg);
        break;
    }
  }
}

void init_server_addr(int argc, char *argv[], struct sockaddr_in* server_addr) {
  int opt;

  memset(server_addr, 0, sizeof(server_addr));
  server_addr->sin_family = AF_INET;
  server_addr->sin_addr.s_addr = INADDR_ANY;
  server_addr->sin_port = htons(8080);

  while ((opt = getopt(argc, argv, "s:")) != -1) {
    server_addr->sin_port = htons(strtoul(optarg, NULL, 10));
  }
}
