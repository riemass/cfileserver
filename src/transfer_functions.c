#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
  close(write_fd);
  return read_size;
}

int read_request(int socket, char* buffer) {
  char* ptr = buffer;
  int bytesReceived;
  while ((bytesReceived = recv(socket, ptr, 1, 0)) > 0) {
    if (*ptr++ == '\n') break;
  }
  *(--ptr) = '\0';
  return ptr - buffer;
}

int parse_request(char* buffer, struct request* request) {
  char* token;
  token = strtok(buffer, " \t\n");
  if (token == NULL)
    return request->type = INVALID;
  if (0 == strcmp(token, "put"))
    request->type = PUT;
  else if (0 == strcmp(token, "get"))
    request->type = GET;
  else 
    return request->type = INVALID;
  token = strtok(NULL, " \t\n");
  if (token == NULL) 
    return request->type = INVALID;
  request->file_path = token;
  return 0;
}
