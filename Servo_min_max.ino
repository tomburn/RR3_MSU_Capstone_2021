#include <Servo.h>

Servo servotest; // Create object

int pos = 0; // pos is position variable


void setup() {
  // put your setup code here, to run once:
servotest.attach(9, 544, 2420); // attach to pin 9 
// These min/max values are very close to delivering full 180 deg rot
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servotest.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servotest.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}
