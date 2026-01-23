int inputPin 9;
int outputPin 10;
int mode = 1;

// Modes
// mode = 1
//	if input == 1 => output == 1
//	if input == 0 => output == 0
// mode = 0
//	if input == 1 => output == 0
//	if input == 0 => output == 1

void setup(){
	pinMode(inputPin,INPUT);
	pinMode(outputPin,OUTPUT);
}

void loop(){
	float input = digitalRead(inputPin);
	if(input == HIGH){
		if(mode){
			digitalWrite(outputPin,HIGH);
		}else{
			digitalWrite(outputPin,LOW);
		}
	}else{
		if(mode){
			digitalWrite(outputPin,LOW);
		}else{
			digitalWrite(outputPin,HIGH);
		}
	}

}
