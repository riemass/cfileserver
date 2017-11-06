#ifndef REQUEST_TYPE
#define REQUEST_TYPE

enum RequestType {
  INVALID = -1,
  PUT = 0,
  GET = 1,
};

struct request {
  enum RequestType type;
  char* file_path;
};

#endif /* ifndef REQUEST_TYPE */
