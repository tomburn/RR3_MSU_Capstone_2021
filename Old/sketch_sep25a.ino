// Servo - Version: 1.1.8
#include <Servo.h>

/*
  This code is to test motors for functionality, and unloaded speed
*/

Servo testservo; // create servo object
int pos = 0; // store servo position

void setup() {
  
  // Define pins that will be used, ports with ~ are PWM outputs
  testservo.attach(9); // check port number, attaches servo on pin 9 to servo object


}

void loop() {
  // Motor test 1: incremental step size forward and back
  for ( pos = 0; pos <= 180; pos += 1) {
    // 90 deg sweep, 1 deg increments
    testservo.write(pos);
    delay(15); // 15ms delay, sweep in 1 dir should be 15*90ms
  }
  for ( pos = 90; pos >= 0; pos -= 1) {
    // 90 deg sweep, 1 deg increments
    testservo.write(pos);
    delay(15) ;// 15ms delay, sweep in 1 dir should be 15*90ms
  }

  // Motor test 2: max speed unloaded for full sweep forward and back
  for ( pos = 0; pos <= 90; pos += 90) {
    // 90 deg sweep, 1 deg increments
    testservo.write(pos);
    delay(15) ;// 15ms delay, sweep in 1 dir should be 15*90ms
  }
  for ( pos = 90; pos >= 0; pos -= 90) {
    // 90 deg sweep, 1 deg increments
    testservo.write(pos);
    delay(15) ;// 15ms delay, sweep in 1 dir should be 15*90ms
  }
  //exit(0)// Break
}
