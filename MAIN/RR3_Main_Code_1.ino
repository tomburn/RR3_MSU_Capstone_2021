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
//#include "arduino_secrets.h" // This is the new tab, normally contains next two lines
#define SECRET_SSID "MSU-Guest"
#define SECRET_PASS ""
char ssid[] = SECRET_SSID;        // your network SSID (name)

// BREAKOUT BOARD
#define nbPCAServo 8 // The number of servos
//Parameters
int MIN_IMP [nbPCAServo] ={544, 544, 544, 544, 544, 544, 544, 544};
int MAX_IMP [nbPCAServo] ={2420, 2420, 2420, 2420, 2420, 2420, 2420, 2420};
//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver(0x40);
////////////////////////////////////////////////////////////////////////////////////////////
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
float swinglf = 0; // create variable position for left front LEG position
float extlf   = 0; // create variable position for left front EXT position
float swingrf = 0; // create variable position for right front LEG position
float extrf   = 0; // create variable position for right front EXT position
float swinglr = 0; // create variable position for left rear LEG position
float extlr   = 0; // create variable position for left rear EXT position
float swingrr = 0; // create variable position for right rear LEG position
float extrr   = 0; // create variable position for right rear EXT position

// These need to stay until the map gets better
int swing;
int ext;
int spos_PWM;

int spd;    // Joystick Y position var - speed input
int turn;   // Joystick X position var - turning input
int adjspd;   // Servo speed Adjusted, int because map() outputs ints

long int interval = 10000; // wait, will be overwritten

unsigned long start_ms;
unsigned long new_ms;
volatile int n; // counter variable

bool swingmotion = true;

// BLYNK CODE
BLYNK_WRITE(V10) {
  
// JOYSTICK INPUTS 
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0
  adjspd = map(spd, -128, 128, 20, 30);
}


// SETUP FUNCTION 
void setup()
{
  
  Serial.begin(9600); // Debug console
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); //Blynk.begin(auth, ssid, pass);
    
  // PCA SETUP
  pca.begin();
  pca.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  
  spos_PWM = map(spos, 0, 180, 544, 2420); // Set all servos to start position
  
  for (i=0; i < nbPCAServo; i++){
               //PCA Commands
               Serial.print("Startup interval ");
               Serial.println(i);
               pca.writeMicroseconds(i,spos); // Swing servo position write to PCA
               for (n=0; n<interval; n++);{}  // Wait loop
               pca.setPin(i,0,true);          // Deactivate Pin i
  }
}





void servomove() // SERVO LOOP
{


  
  if (spd > dead_fwd){           //FORWARD MOTION
    Serial.print("Forward");  
      if (turn > dead_right){    // RIGHT TURN
          Serial.print("Right");      
          }
      else if (turn < dead_left){// LEFT TURN
          Serial.print("Left");
          }
      else {                     // STRAIGHT   
      Serial.println("Straight");

      // SERVO CONTROL SECTION
//      if (swingmotion = true){
      while (swing < swing_max){                          // STEP 
        swing=swing+adjspd;
               
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
//               swingrr = map(swing, 0, 180, 544, 2420);
//               extrr   = map(ext,   0, 180, 544, 2420);
//               pca.writeMicroseconds(2,swingrr);         // Swing servo position write to PCA
//               pca.writeMicroseconds(3,extrr);           // Extension servo position write to PCA
               
               for (n=0; n<interval; n++);{}             // Delay loop
               
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
               
               }
              
      }
//      swingmotion != swingmotion;
//      }
      while (swing > swing_min){                         // SWING
        swing=swing-adjspd;
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
//               swingrr = map(swing, 0, 180, 544, 2420);
//               extrr   = map(ext,   0, 180, 544, 2420);
//               pca.writeMicroseconds(2,swingrr);         // Swing servo position write to PCA
//               pca.writeMicroseconds(3,extrr);           // Extension servo position write to PCA
               
               for (n=0; n<interval; n++);{}             // Delay loop
               
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               //PCA Commands
               swinglf = map(swing, 0, 180, 544, 2420);
               extlf   = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,swinglf);         // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf);           // Extension servo position write to PCA
               for (n=0; n<interval; n++);{}             // Delay loop
               
               }
      } 
//      swingmotion != swingmotion;
      
    }
    }
 
  
  else if (spd < dead_back) {                   // BACKWARD MOTION
   Serial.print("Backward");
  
    if (turn > dead_right){
      Serial.print("Right");     
      }
    else if (turn < dead_left){
      Serial.print("Left");
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




void loop() // MAIN LOOP
{
  Blynk.run();
  servomove();
}
