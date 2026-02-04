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

	if(!commandsFile){
		fprintf(stderr,"CAN'T OPEN FILE %s","commands.csv");
		return 1;
	}

	int c;
	int fd = serialOpen("/dev/serial0",9600);

	if(fd < 0){
		fprintf(stderr,"CAN'T OPEN SERIAL PORT\n");
		fclose(commandsFile);
		return 1;
	}

	char line[64];
	while(fgets(line, sizeof(line), commandsFile) != NULL){
		serialPuts(fd, line);
	}

	fclose(commandsFile);
	serialClose(fd);
	return 0;
}
