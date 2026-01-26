#include <Stepper.h>

int inputPin = 12;

const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(9600);
}

void loop() {
  char c;
  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.print("Received: ");
    Serial.println(c);
  }
  if(c == 'f'){
    Serial.println("clockwise");
    myStepper.step(1);
    delay(10);
  }
  if(c == 'b'){
    Serial.println("counterclockwise");
    myStepper.step(-1);
    delay(10);
  }
}

