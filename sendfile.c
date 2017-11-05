#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int server;
  int read_fd;
  struct sockaddr_in server_addr;
  struct stat stat_buf;
  off_t offset = 0;
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

  read_fd = open(file_path, O_RDONLY);
  fstat(read_fd, &stat_buf);
  sendfile(server, read_fd, &offset, stat_buf.st_size);

  close(server);
  close(read_fd);
  return 0;
}
