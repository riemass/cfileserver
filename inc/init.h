#ifndef INIT_H_
#define INIT_H_ value

void init_client_addr(int argc, char *argv[], struct sockaddr_in* server_addr);
void init_server_addr(int argc, char *argv[], struct sockaddr_in* server_addr);

#endif /* ifndef INIT_H_ */
