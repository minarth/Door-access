#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#ifdef _WIN32
 #include <winsock.h>
 #include <fcntl.h>
 typedef int socklen_t;
#else
 #include <sys/select.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #define closesocket(s)  close(s)
#endif /* _WIN32 */
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

#define FALSE		0
#define TRUE		1

unsigned char * base;

struct sockaddr_in server_address;
struct hostent *server;

int pciEnable(int isEnable);
int lookupDevice();
int findAddress();
