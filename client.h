#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdbool.h>

#define PORT_NUMBA	55556

#define RESPONSE_CODE_OK			0
#define RESPONSE_CODE_UNKNOWN		-1
#define RESPONSE_CODE_UNAUTH		1
#define RESPONSE_CODE_WRONGPW		2
#define RESPONSE_CODE_WRONGID		3

int connectToServer(char* server_ip);
void closeConnection();
void sendMessageToServer(char* command);
int evaluateResponse(char * response);
void responseOfServer(int code);