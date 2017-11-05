#ifndef TRANSFER_FUNCTIONS_H_
#define TRANSFER_FUNCTIONS_H_

int send_file(int socket, const char* file_path);
int recv_file(int socket, const char* file_path);

#endif /* ifndef TRANSFER_FUNCTIONS_H_ */
