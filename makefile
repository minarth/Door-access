all:
	gcc main.c functions.c client.h finddevice.c -o main
	sudo ./main