#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include <pthread.h>

#include "init.h"
#include "request_type.h"
#include "transfer_functions.h"

#define BACKLOG 10

void* handle_connection_wrap(void*);

int main(int argc, char *argv[]) {
  int server;
  intptr_t client;
  int c;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  init_server_addr(argc, argv, &server_addr);

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
  while (client =
             accept(server, (struct sockaddr *)&client_addr, (socklen_t *)&c)) {

    /* handle_connection_wrap((void *)client); */

    pthread_t thread;
    pthread_create(&thread, NULL, &handle_connection_wrap, (void*)client);
    pthread_detach(thread);
  }

  close(server);

  return 0;
}

void handle_connection(intptr_t client) {
  char buffer[80];
  struct request req;

  read_request(client, buffer);
  parse_request(buffer, &req);
  if (req.type == PUT) {
    recv_file(client, req.file_path);
    printf("Connection accepted. Receiving file: %s.\n", req.file_path);
  } else if (req.type == GET) {
    send_file(client, req.file_path);
    printf("Connection accepted. Sending file: %s.\n", req.file_path);
  } else {
    perror("Error: unrecognized method.");
  }
}

void *handle_connection_wrap(void *arg) {
  intptr_t client = (intptr_t)arg;
  if (client < 0) {
    perror("Accept failed. Error");
  } else {
    handle_connection(client);
    close(client);
  }
  return NULL;
}
