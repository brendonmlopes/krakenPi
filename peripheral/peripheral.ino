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
long pos = 0;

void setup() {
  stepper.setSpeed(80);
  Serial.begin(9600);
  Serial.println("START");
}

static long readNumber(const char* s, int& i, int maxLen) {
  long sign = 1;
  if (i < maxLen && s[i] == '-') { sign = -1; i++; }
  long v = 0;
  while (i < maxLen && s[i] >= '0' && s[i] <= '9') {
    v = v * 10 + (s[i] - '0');
    i++;
  }
  return sign * v;
}

void loop() {
  if (!Serial.available()) return;

  char msg[64];
  int len = Serial.readBytesUntil('\n', msg, sizeof(msg) - 1);
  msg[len] = '\0';
  
  if (len < 3 || len >= sizeof(msg)) return;

  // Parse: ID,MULT,CMD,VALUE
  int i = 0;

  long id = readNumber(msg, i, len);
  if (i >= len || msg[i] != ',') return;
  i++;

  long mult = readNumber(msg, i, len);
  if (i >= len || msg[i] != ',') return;
  i++;

  if (i >= len) return;
  char cmd = msg[i++];

  if (i >= len || msg[i] != ',') return;
  i++;

  long value = readNumber(msg, i, len);

  if (id != MY_ID) {
    Serial.println("ERR:BAD_ID");
    return;
  }
  if (mult <= 0) mult = 1;
  if (mult > 10000) mult = 10000;
  
  if (value > 32767 || value < -32767) {
    Serial.println("ERR:VALUE_OOR");
    return;
  }

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
        Serial.print("ERR:BAD_CMD:");
        Serial.println(cmd);
        break;
    }
  }
}
