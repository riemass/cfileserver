#ifndef TRANSFER_FUNCTIONS_H_
#define TRANSFER_FUNCTIONS_H_

#include <string.h>

#include "request_type.h"


int send_file(int socket, const char* file_path);
int recv_file(int socket, const char* file_path);
int read_request(int socket, char* buffer);
int parse_request(char* buffer, struct request* request);

#endif /* ifndef TRANSFER_FUNCTIONS_H_ */
