#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

#include "transfer_functions.h"

#define BUFSZ 1024

int send_file(int socket, const char* file_path) {
  int read_fd;
  struct stat stat_buf;
  off_t offset = 0;

  read_fd = open(file_path, O_RDONLY);
  fstat(read_fd, &stat_buf);
  sendfile(socket, read_fd, &offset, stat_buf.st_size);
  close(read_fd);
  return 0;
}

int recv_file(int socket, const char* file_path) {
  int write_fd;
  int read_size;
  mode_t mode = 0644;
  char buffer[BUFSZ];

  write_fd = open(file_path, O_WRONLY | O_CREAT, mode);
  while ((read_size = recv(socket, buffer, BUFSZ, 0)) > 0) {
    write(write_fd, buffer, read_size);
  }
  if (read_size == 0) fsync(write_fd);
  return read_size;
}
