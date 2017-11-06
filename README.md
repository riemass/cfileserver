
File sharing server and client.
Server is concurrent, supporting configurable port.
> server [-p <port>]

Client is sequential, running in a loop and evaluating the following methods:
```
get <filename>
put <filename>
```
Configurable address and port:
> client [-a <address> -p <port>]

If not specified, client and server run on localhost and port 8080.
