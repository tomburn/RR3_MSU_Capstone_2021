// SETUP

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLR_TV2ZqB"
#define BLYNK_DEVICE_NAME "servoposition"
#define BLYNK_AUTH_TOKEN "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <Servo.h> 
#include <math.h> 
#include <Wire.h> //https://www.arduino.cc/en/reference/wire
#include <Adafruit_PWMServoDriver.h> //https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

char auth[] = "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
//char ssid[] = "MSU-Guest";
char pass[] = "";
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;        // your network SSID (name)

// BREAKOUT BOARD
#define nbPCAServo 8 // The number of servos
//Parameters
int MIN_IMP [nbPCAServo] ={544, 544, 544, 544, 544, 544, 544, 544};
int MAX_IMP [nbPCAServo] ={2420, 2420, 2420, 2420, 2420, 2420, 2420, 2420};
//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40);

// Constants
const int spos= 90; // Start position 90 deg for all legs
const int dead_fwd   = 20; // Joystick Deadzone
const int dead_back  = -20;
const int dead_left  = -20;
const int dead_right = 20;

const int swing_max = 135; // Maximum Swing Servo Position
const int swing_min = 45;  // Minimum Swing Servo Position
const int ext_max   = 180; // Mximum Extension Servo Position
const int ext_min   = 45;  // Minimum Extension Servo Position


// VARIABLES
int i = 0; // Servo Index
// SPECIFIC TO REPLACE GLOBAL 
int swinglf = 0; // create variable position for left front LEG position
int extlf   = 0; // create variable position for left front EXT position
int swingrf = 0; // create variable position for right front LEG position
int extrf   = 0; // create variable position for right front EXT position
int swinglr = 0; // create variable position for left rear LEG position
int extlr   = 0; // create variable position for left rear EXT position
int swingrr = 0; // create variable position for right rear LEG position
int extrr   = 0; // create variable position for right rear EXT position

// These need to stay until the map gets better
int swing;
int ext;

int spd;    // Joystick Y position var - speed input
int turn;   // Joystick X position var - turning input
int adjspd;   // Servo speed Adjusted, int because map() outputs ints

int interval = 2000; // 2 sec wait, will be overwritten

unsigned long start_ms;
unsigned long new_ms;
volatile int n; // counter variable

// BLYNK CODE
BLYNK_WRITE(V10) {
  
// JOYSTICK INPUTS 
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0
  adjspd = map(spd, 20, 128, 0, 20);
}


// SETUP FUNCTION 
void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
    
  // PCA SETUP
  pca.begin();
  pca.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  // Set all servos to start position
  spos = map(90, 0, 180, 544, 2420);
  for (i=0; i < nbPCAServo; i++);{
               //PCA Commands
               Serial.print("Startup interval ");
               Serial.println(i);
               pca.writeMicroseconds(i,spos); // Swing servo position write to PCA
               for (n=0; n<interval; n++);{}  // Wait loop
               pca.setPin(i,0,true);          // Deactivate Pin i
               i=i+1; 
  }
    i=0; // Reset i to start position after sweeping through each servo. 
}



void loop() // MAIN LOOP
{
  Blynk.run();
  servomove();
}



void servomove() // SERVO LOOP
{
  if (spd > dead_fwd){                        //FORWARD MOTION
  Serial.print("Forward");
  
  /*  
  if (turn > dead_right){                     // RIGHT TURN
      Serial.print("Right");
                                        // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
              for (swing = 45; swing <= 135; swing += adjspd) { // SERVO POSITION CHANGE STEP LOOP
                 
              if (swing <= 90) {                        // Split EXT Servo into step chunks
              ext= swing+45;                            // EXT for step squence
              }
              else {
              ext= 135-(swing-90);                     // EXT for step sequence  
              }
             
         }
         for (swing = 135; swing >= 45; swing -= adjspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
         }
         prevMillis=currMillis; // update time 
      //}
    
    }
    else if (turn < dead_left){                // LEFT TURN
      Serial.print("Left");
                  // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
      for (swing = 45; swing <= 135; swing += adjspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
         }
         for (swing = 135; swing >= 45; swing -= adjspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
         }
         prevMillis=currMillis; // update time 
      //}
    }
    else {
    // STRAIGHT     
*/
    
      Serial.println("Straight");

    // SERVO CONTROL SECTION
      

       for (swing = swing_min; swing <= swing_max; swing += adjspd) { // SERVO POSITION CHANGE STEP LOOP
             
             if (swing <= 90) {                          // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
              }
             
              else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf); // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf); // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
             }
           
               
         }
         for (swing = swing_max; swing >= swing_min; swing -= adjspd) { // SERVO POSITION CHANGE SWING LOOP    
             if (swing >= 90) {                         // Split EXT Servo into step chunks
               ext= 90+2*(-swing-135);                    // EXT for swing squence
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
             }
             else {
               ext= 180+2*(swing-90);                    // EXT for swing sequence
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
             }

             
         }
    }
  
 
  
  else if (spd < dead_back) {                   // BACKWARD MOTION
   Serial.print("Backward Speed = ");
  Serial.print(spd);
  Serial.println(" ");
  
    if (turn > 20){
      Serial.print("Right");
      Serial.print(turn);
      Serial.println(" ");
    }
    else if (turn < -20){
      Serial.print("Left");
      Serial.print(turn);
      Serial.println(" ");
    }
    else {
      Serial.print("Straight");
      
    }
  }
 
  
  else {
    //Serial.println("Stopped");      // NO MOTION
    n=n+1;
    
    if (n>100){         // No motion for a while, move legs back to neutral
      if (swing != 90)
          ext = 90;
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf); // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf); // Extension servo position write to PCA
          
          swing = 90; 
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf); // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf); // Extension servo position write to PCA
      n=0;
    }
    }
  
}
