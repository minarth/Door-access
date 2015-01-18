
#include "finddevice.h"
#include "My_Header.h"
#include "client.h"

unsigned int deviceAddress = 0;
unsigned char devEnable[41] = "/sys/bus/pci/devices/0000:0i:jk.l/enable";

int pciEnable(int isEnable) {
	char cen = isEnable!=0 ? '1' : '0';
	int enable = open(devEnable, O_WRONLY);
	if(enable == -1) {
		printf("Error: PCI is not enabled. Are you root?\n");
		return 0;
	}
	write(enable, &cen, 1); 
	close(enable);
	return 1;
}

int lookupDevice() {
	
	char devise[] = "/proc/bus/pci/0i/jk.l";
	int readSize = 32;
	int c;

	int readCorrectly = 0;
	int w = 0;
	char buffer2[8];
	char manu[5];
	char code[5];							

	unsigned int manufacturer;
	unsigned int device;
	unsigned int address;
	unsigned int buffer[readSize];

	int i, j, k, l;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			for (k = 0; k < 10; k++) {
				for (l = 0; l < 10; l++) {
					
					FILE *file;
					devise[15] = (char)(((int)'0') + i);
					devise[17] = (char)(((int)'0') + j);
					devise[18] = (char)(((int)'0') + k);
					devise[20] = (char)(((int)'0') + l);
					file = fopen(devise, "r");
					if (file) {
						readCorrectly = 0;
						readCorrectly = fread(&buffer, sizeof(unsigned int), readSize, file);
						w = 0;
						buffer2[8];
						
						strncpy(code, buffer2, 4);
						strncpy(manu, buffer2+4, 4);
						manu[4] = '\0';
						code[4] = '\0';

						manufacturer = (int)strtol(manu, NULL, 16);
						device = (int)strtol(code, NULL, 16);
						address = buffer[4];

						if (manufacturer == DEV_MANUFACTURER && device == DEV_CODE) {
							
							deviceAddress = address;
							devEnable[27] = (char)(((int)'0') + i);
							devEnable[29] = (char)(((int)'0') + j);
							devEnable[30] = (char)(((int)'0') + k);
							devEnable[32] = (char)(((int)'0') + l);

							return 1;
							
						}
						fclose(file);
					}					
				}			
			}
		}
	} 
	return 0;
}

int findAddress() {
	
	int file= open("/dev/mem", O_RDWR | O_SYNC);
	if(file == -1) {
		printf("Error: File not found\n");
		return 1;
	}
	lookupDevice();
	base = mmap(NULL, 0x10000, PROT_WRITE | PROT_READ, MAP_SHARED, file, deviceAddress);
	if(base == MAP_FAILED) {
		printf("Error: base not found\n");	
		return 2;
	}
	return 0;
}


void wrongAccess() {
	writeInAddress(0x03, 0x90);
	sleep(1);
	writeInAddress(0x03, 0x7f);
	sleep(1);
	writeInAddress(0x03, 0x90);
	sleep(1);
	writeInAddress(0x03, 0x7f);
	sleep(1);
	writeInAddress(0x03, 0x90);
	sleep(1);
	writeInAddress(0x03, 0x7f);
	sleep(2);
	clearDisplay();
}




