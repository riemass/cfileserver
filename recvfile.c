#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BACKLOG 3
#define BUFSZ 1024

int main(int argc, char *argv[]) {
  int server;
  int client;
  int write_fd;
  int c;
  int read_size;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  mode_t mode = 0644;
  char buffer[BUFSZ];
  char *file_path;
  int opt;

  if (argc < 2) {
    puts("Usage:\n\trecvfile [-s socket] path");
    exit(1);
  }
  file_path = argv[argc - 1];

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

  puts("Waiting for incoming file...");

  c = sizeof(struct sockaddr_in);
  client = accept(server, (struct sockaddr *)&client_addr, (socklen_t *)&c);
  if (client < 0) {
    perror("Accept failed. Error");
    exit(1);
  }
  puts("Connection accepted. Writing incoming file.");

  write_fd = open(file_path, O_WRONLY | O_CREAT, mode);
  while ((read_size = recv(client, buffer, BUFSZ, 0)) > 0) {
    write(write_fd, buffer, read_size);
  }
  if (read_size == 0) {
    puts("Receive succesfoul.");
    fsync(write_fd);
  } else if (read_size < 0) {
    perror("Receive of file failed. Error");
  }
  close(server);
  close(write_fd);

  return 0;
}
