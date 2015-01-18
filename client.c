#include "client.h"

int server_connected = 0;

int socket_fd;                      //socket file descriptor
struct sockaddr_in server_address;
struct hostent *server;

unsigned char RESPONSE_OK[2] = "OK";
unsigned char RESPONSE_FRBDN_USER[24] = "forbidden hint \"user id";
unsigned char RESPONSE_FRBDN_PW[14] = "pin missmatch";
unsigned char RESPONSE_FRBDN_UNAUTH[21] = "not allowed for gate";

void responseOfServer(int code) {
	switch(code) {

		case RESPONSE_CODE_OK: 

		writeInAddress(0x03, 0x90);
		writeDisplay("ACCESS GRANTED!", "");
		usleep(1);
		writeInAddress(0x03, 0x7f);
		sleep(2);
		clearDisplay();
		break;
				
		case RESPONSE_CODE_WRONGID: 
		writeDisplay("UNKNOWN ID", "");
		wrongAccess();
		break;
		
		case RESPONSE_CODE_WRONGPW: 
		writeDisplay("WRONG PIN", "");
		wrongAccess();
		break;
		
		case RESPONSE_CODE_UNAUTH: 
		writeDisplay("NOT AUTHORIZED", "");
		wrongAccess();
		break;
		
		default: 
		writeDisplay("UNKNOWN COMMAND", "");
		wrongAccess();
		break;
		
	}
}


int evaluateResponse(char * response) {
	int counter = 0, wsCounter = 0;

	while(*(response + counter) != '\0') {
		if (*(response + counter) == ' ') {
			wsCounter++;
		}
		if (wsCounter == 3) {
			char * equal = strstr((response + counter), RESPONSE_OK);
			if (equal != NULL) {
				return RESPONSE_CODE_OK;			
			}
			equal = strstr((response + counter), RESPONSE_FRBDN_USER);
			if (equal != NULL) {				
				equal = strstr((response + counter), RESPONSE_FRBDN_UNAUTH);
				if (equal != NULL) {
					return RESPONSE_CODE_UNAUTH;					
				}			
				equal = strstr((response + counter), RESPONSE_FRBDN_PW);
				if (equal != NULL) {
					return RESPONSE_CODE_WRONGPW;
				}	
				return RESPONSE_CODE_WRONGID;
			}			
		}
		counter++;
	}
	return RESPONSE_CODE_UNKNOWN;
}

/* Sends message to server and translate response */
void sendMessageToServer(char* command) {

    int n = 0, response = 0;

    printf("Sending: %s ",command);
    n = write(socket_fd, command, strlen(command));
	
    char buffer[256];
    bzero(buffer, 256);
	response = read(socket_fd, buffer, 256);

    if (response < 0) {
		printf("Server is not responding.");
    } else {
		printf("Response: %s", buffer);
		int errorCode = evaluateResponse(buffer);
		responseOfServer(errorCode);
	}
}

/* Connects the client to server. */
int connectToServer(char* server_ip) {
    
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* If socket system call fails, it returns -1. */
    if (socket_fd < 0) {
        printf("There was error opening a socket.");
        return 1;     
    }

    /* Finds server by name. */
    server = gethostbyname(server_ip);

    /* If server of such name was not found. */
    if (server == NULL) {
        printf("Server was not found.\n");
        return 1; 
    }

    /* Sets the fields in serv_addr. */
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(PORT_NUMBA);

    if (connect(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("Failed to connect.\n");
        return 1; 
    }

    printf("Connection established.\n");
    return 0;
}

/* Closes connection. */
void closeConnection() {	
	close(socket_fd);
}
