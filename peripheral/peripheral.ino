#include <Stepper.h>

int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  if(Serial.available()){
    String c = Serial.readStringUntil('\n');
    c.trim();

    if (c.length() < 2){
      return;
    }
    char dir = c[c.length() - 1];
    int steps = c.substring(0, c.length() - 1).toInt();

    if(dir == 'f'){
      myStepper.step(steps);
    }

    if(dir == 'b'){
      myStepper.step(-steps);
    }

  }
}
