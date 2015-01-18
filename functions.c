#include "My_Header.h"

char previous_char = 0; 
char current_char = 0; 
int counta;

// reading cycle from address of keyboard
unsigned char readK(void) { 
	unsigned char tmp; 
	*(base+ADD) = 0x00; 
	*(base+CTRL) = 0xfe; 
	*(base+CTRL) = 0xbe; 
	usleep(DELAY); 
	tmp = ~(*(base+0x8040)); 
	*(base+CTRL) = 0xfe; 
	*(base+CTRL) = 0xff; 
	return(tmp);
}

// write i into keyboard in register
void writeK(char i) { 
	writeInAddress(0x03, i); 
}

// function for reading from keyboard
// it activates columns 1, 2, 3 and than
// reads from rows, which key is active
int keyboard() { 
	unsigned int status = 0, slKeyb; 
	writeK(0x06); 
	status = (readK()&0x1F); 
	writeK(0x05); 
	status = (status << 5) | (readK()&0x1F); 
	writeK(0x03); 
	status = (status << 5) | (readK()&0x1F); 
	return(status); 
}

// transform table for code from keyboard 
// to its assigned value
char readChar(int code) { 
	//printf("T: %i\n", code);
	switch (code) { 
		case 0: 
		return('-'); 
		break; 
		case 1: 
		return('1'); 
		break; 
		case 2: 
		return('4'); 
		break; 
		case 4: 
		return('7'); 
		break; 
		case 8: 
		return('.'); 
		break; 
		case 16:
		return('E');
		break;
		case 32: 
		return('2'); 
		break; 
		case 64: 
		return('5'); 
		break; 
		case 128: 
		return('8'); 
		break; 
		case 256: 
		return('0'); 
		break; 
		case 512:
		return('G');
		break;
		case 1024: 
		return('3'); 
		break; 
		case 2048: 
		return('6'); 
		break; 
		case 4096: 
		return('9'); 
		break; 
		case 8192: 
		return('*'); 
		break; 
		default: 
		return('@'); 
		break; 
	} 
}

// checking, if user holds quit for one second
// if so - than programm ends and device shut down
int endingCycle() {
	int i = 0;
	
	while (i < 5 && current_char == 'E') {
		usleep(200000);
		i++;
		current_char = readChar(keyboard()); 
	}
	if (i == 5) {
		printf("END!\n" );

		return -1;
	}
	previous_char = current_char; 
	return 0;
}

// function reading character from keyboard
// it prevents multiple reading of same char
// caused by some electromagnetic noise, double clicks, etc.
char readKeyboard() { 
	while(TRUE) { 
		current_char = readChar(keyboard()); 
		
		//printf("%c\n", current_char);
		if (current_char != previous_char) { 
			previous_char = current_char; 
			counta = 1; 
		} else if(current_char=='@' || current_char=='-') { 
			counta = 0; 
		} else if(current_char=='E') {
			if (endingCycle() == -1) {
				break;
			}

		}
		else { 
			if(counta < 1) { 
				counta++; 
			} else if (counta == 1) { 
				counta++;


				if(current_char == '-') { 
					writeInAddress(0x03, 0x7f); 
				} else { 
					writeInAddress(0x03, 0x90); 
				}

				return current_char; 
			} 
		} 
	}
	return -1; 
}

// method for writing data into the address on device
void writeInAddress(int address, int data) { 
	*(base+ADD)=address; 
	*(base+DATA)=data; 
	*(base+CTRL)=0xFD; 
	*(base+CTRL)=0xBD; 
	usleep(DELAY); 
	*(base+CTRL)=0xFD; 
	*(base+CTRL)=0xFF; 
}
 
void clearDisplay() { 
	writeInAddress(0x00, 0x01); 
} 

void writeChar(char c) { 
	writeInAddress(0x02, c); 
} 

void writeLine(char * line) { 
	int counter = 0; 
	while (*(line + counter) != '\0') { 
		writeChar(*(line + counter)); 
		counter++; 
	} 
} 

void writeDisplay(char * firstRow, char * secondRow) { 
	clearDisplay(); 
	writeLine(firstRow); 
	writeInAddress(0x00, 168); 
	writeLine(secondRow); 
}

int readNumba(char x) {
	if (x >= 48 && x <= 57) {
		return (x-48);
	} else {
		return -1;
	}
}

/* Prepare device into working state*/
void turnON() {
		*(base+CTRL)=0xFF;
		usleep(5000000);
		writeInAddress(0x03, 0x00);
		writeInAddress(0x00, 0x38);
		writeInAddress(0x00, 0x38);
		writeInAddress(0x00, 0x01);
		writeInAddress(0x00, 0x0C);	 
}

