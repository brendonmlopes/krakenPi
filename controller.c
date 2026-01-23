#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define maxPins 26
#define C_RED "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_YELLOW "\033[1;33m"
#define C_RESET "\033[0m"

typedef struct Pin{
	int inputPin;
	int input;
	int outputPin;
	int output;
}Pin;

int main(int argc, char* argv[]){

	if(argc==1){
		fprintf(stderr,C_RED "\n\nSYNTAX ERROR : Not enough arguments\nUsage:\tkrakenpi [INPUT_PIN1]=[INPUT_VALUE1],[OUTPUT_PIN1]=[OUTPUT1] [INPUT_PIN2]=[INPUT_VALUE2],[OUTPUT_PIN2]=[OUTPUT2] ... \n" C_RESET);
		return 1;
	}

	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 3000000000000;

	wiringPiSetupGpio();
	Pin pins[maxPins];

	for(int i = 0 ; i < maxPins ; i++){
		pins[i].inputPin = -1;
		pins[i].outputPin = -1;
		pins[i].input = -1;
		pins[i].output = -1;
	}

	printf(C_YELLOW "\n================================\nCommands received:\n================================\n\n" C_RESET);
	for(int i = 1; i < argc ; i++){
		char* command = argv[i];
		printf("%d: %s\n",i,command);

		int inputPin;
		int outputPin;
		int input;
		int output;

		sscanf(command,"%d=%d,%d=%d",&inputPin,&input,&outputPin,&output);

		if(!((input == 0 || input == 1) && (output == 0 || output == 1))){
			fprintf(stderr,C_RED "\n\nSYNTAX ERROR ON COMMAND %d: INPUT AND OUTPUT MUST BE 0 OR 1\n",i);
			fprintf(stderr,"input:\t%d\toutput:\t%d\n" C_RESET ,input,output);
			return 1;
		}

		if(inputPin<0 || inputPin > maxPins || outputPin<0 || outputPin > maxPins){
			fprintf(stderr,C_RED "\n\nSYNTAX ERROR ON COMMAND %d: PINS MUST BE BETWEEN 0 AND %d\n",i,maxPins);
			fprintf(stderr,"input pin:\t%d\toutput pin:\t%d\n" C_RESET,inputPin,outputPin);
			return 1;
		}

		Pin pin;
		pin.inputPin = inputPin;
		pin.outputPin = outputPin;
		pin.input = input;
		pin.output = output;

		pins[i-1] = pin;

	}

	printf(C_YELLOW "\n================================\nExecuting...\n================================\n\n" C_RESET);

	for(int i = 0 ; i < maxPins ; i++){
		Pin pin = pins[i];
		if(pin.inputPin <= -1 || pin.input <= -1 || pin.outputPin <= -1 || pin.output <= -1){
			continue;
		};


		pinMode(pin.inputPin,INPUT);
		pinMode(pin.outputPin,OUTPUT);
		printf("Setting pinMode...");
		printf("inputPin:%d->%d -> outputPin:%d->%d\n",pin.inputPin,pin.input,pin.outputPin,pin.output);

		nanosleep(&ts,NULL);

		int input = digitalRead(pin.inputPin);
		if(input == pin.input){
			digitalWrite(pin.outputPin,pin.output);
		}else{
			if(pin.output==HIGH){
				digitalWrite(pin.outputPin,LOW);
			}else{
				digitalWrite(pin.outputPin,HIGH);
			}
		}
	}
	printf(C_GREEN "\n================================\nDONE\n================================\n\n" C_RESET);
	
	return 0;
}
