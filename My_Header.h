#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <dirent.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <time.h> 
#include <errno.h> 
#include <netinet/in.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdbool.h>
#include "finddevice.h"
#include "client.h"

#define DEV_MANUFACTURER 0x1172 
#define DEV_CODE 0x1f32 
#define DEV_ENABLE "/sys/bus/pci/devices/0000:03:00.0/enable" 
#define DEV_ADDRESS 0xfe8f0000 
#define NEXT_LINE 168 
#define CTRL 0x8080 
#define ADD 0x8060 
#define DATA 0x8020 
#define DELAY 10000 
#define FALSE 0 
#define TRUE 1 
#define TURN_OFF 0 
#define TURN_ON 1 
#define BEEP_FACTOR	1000

volatile int tmp; 
unsigned char * base;
unsigned char readK(void); 
void writeK(char i); 
int keyboard();
char readChar(int code);
int endingCycle();
char readKeyboard();
void writeInAddress(int address, int data);
void clearDisplay();
void writeChar(char c);
void writeLine(char * line);
void writeDisplay(char * firstRow, char * secondRow);
int readNumba(char x);
void turnON(int state);
