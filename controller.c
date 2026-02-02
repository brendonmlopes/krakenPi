#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define maxPins 26

typedef struct command{
	int outputPin;
}command;

int main(int argc, char* argv[]){
	FILE* commandsFile = fopen("commands.csv","r");
	int fd = serialOpen("/dev/serial0",9600);

	int c;

	if(!file){
		fprintf(stderr,"CAN'T CREATE FILE %s","commands.csv");
		return 1;
	}

	char line[30];

	while((c=fgetc(file))!=EOF){
		serialPuts(fd,c);
	}

	fclose(file);
	serialClose(fd);
	return 0;
}
