
#include "My_Header.h"


int insertID() {
	clearDisplay();
	int id = 0;
	int tmp;
	writeDisplay("INSERT ID:", "");
	char t = '2'; 
	while(t != 'G') { 
		t = readKeyboard();
		if (t == -1) {return -1;} 
		tmp = readNumba(t);
		writeChar(t); 
		printf("%c\n", t); 
		if (tmp > -1) {
			id = id*10 + tmp;
		}
	}

	return id;
}

int insertPIN() {
	clearDisplay();
	int pin = 0;
	int tmp;
	writeDisplay("INSERT PIN:", "");
	char t = '2'; 
	while(t != 'G') { 
		t = readKeyboard(); 
		if (t == -1) {return -1;} 
		tmp = readNumba(t);
		writeChar('*'); 
		printf("%c\n", t); 
		if (tmp > -1) {
			pin = pin*10 + tmp;
		}
	}

	return pin;
}

int checkAccess(int id, int pin) {
	sendMessageToServer("checkaccess 4 %i %i\n", id, pin);
	sleep(2);
}

int lifeCycle() {
	while(TRUE) {
		int id = insertID();
		if (id == -1) {break;}
		printf("ID : %i\n", id);
		int pin = insertPIN();
		if (pin == -1) {break;}
		printf("PIN : %i\n", pin);
		checkAccess(id, pin);
	}

}

int main() { 

	int error = findAddress();
	if (error != 0) {
		return error;
	}

    int checkConnection = 0;
	checkConnection = connectToServer("localhost");
	
	if (checkConnection != 0) {
		printf("Error connecting to server.\n");
	}

	if(pciEnable(TRUE)) {
		turnON(TURN_ON);

		lifeCycle();
		
		closeConnection();
		*(base+CTRL) = 0x00;
		
	}

	pciEnable(FALSE);
	return 0;
}
