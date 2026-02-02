// PERIPHERAL.INO
// This is the arduino reciever logic. It's going to recieve a message from the controller with the following data:
// 1: ID.
//      If the ID of the reciever is the same as the ID on the message from the controller, then some logic can be executed on that peripheral device.
// 2: MULTIPLE
//      The number of times the command to be executed will be executed. This reduces the need for multiple messages when a command needs to be run multiple times
// 3: COMMAND
//      This is the command the peripheral must execute. The logic for how the command must be executed must be in this file
// 4: END OF MESSAGE
//      To avoid interference with other messsages, the end of each message must contain a byte to indicate it's end.
// EXAMPLE USAGE
//    3,5,F,100 
//  This means the peripheral with id = 3 will repeat action F 5 times with a parameter of 100.
//
#include <Stepper.h>

const int MY_ID = 3;

int stepsPerRevolution = 200;
Stepper stepper(stepsPerRevolution, 8, 9, 10, 11);
int pos = 0;

void setup() {
  stepper.setSpeed(80);
  Serial.begin(115200);
  Serial.println("START");
}

static long readNumber(const String& s, int& i) {
  long sign = 1;
  if (i < s.length() && s[i] == '-') { sign = -1; i++; }
  long v = 0;
  while (i < s.length() && s[i] >= '0' && s[i] <= '9') {
    v = v * 10 + (s[i] - '0');
    i++;
  }
  return sign * v;
}

void loop() {
  if (!Serial.available()) return;

  String msg = Serial.readStringUntil('\n');
  msg.trim();
  if (msg.length() < 3) return;

  // Parse: ID,MULT,CMD,VALUE
  int i = 0;

  long id = readNumber(msg, i);
  if (i >= msg.length() || msg[i] != ',') return;
  i++;

  long mult = readNumber(msg, i);
  if (i >= msg.length() || msg[i] != ',') return;
  i++;

  if (i >= msg.length()) return;
  char cmd = msg[i++];

  if (i >= msg.length() || msg[i] != ',') return;
  i++;

  long value = readNumber(msg, i);

  if (id != MY_ID) return;
  if (mult <= 0) mult = 1;
  if (mult > 10000) mult = 10000;

  for (long k = 0; k < mult; k++) {
    switch (cmd) {
      case 'F': 
        Serial.println("MOVE:F");
        stepper.step((int)value); 
        pos += value; 
        break;
      case 'B': 
        Serial.println("MOVE:B");
        stepper.step(-(int)value); 
        pos -= value ;
        break;
      default: 
        break;
    }
  }
}
