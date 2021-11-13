// SETUP
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLR_TV2ZqB"
#define BLYNK_DEVICE_NAME "servoposition"
#define BLYNK_AUTH_TOKEN "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
//#define BLYNK_TEMPLATE_ID   "TMPLR_TV2ZqB"
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


// VARIABLES
int i = 0; // Servo Index
// GLOBAL TO BE REMOVED:
int swing;
int ext; 
long int spos; 
long int epos;
/* SPECIFIC TO REPLACE GLOBAL 
float swinglf = 0.0; // create variable position for left front LEG position
float extlf = 0.0; // create variable position for left front EXT position

float swingrf = 0.0; // create variable position for right front LEG position
float extrf = 0.0; // create variable position for right front EXT position

float swinglr = 0.0; // create variable position for left rear LEG position
float extlr = 0.0; // create variable position for left rear EXT position

float swingrr = 0.0; // create variable position for right rear LEG position
float extrr = 0.0; // create variable position for right rear EXT position

*/

long int spd;    // Joystick Y position var - speed input
long int turn;   // Joystick X position var - turning input
long int sspd; // Servo speed Adjusted, int because map() outputs ints


float interval = 2000; // 2 sec wait, will be overwritten

unsigned long start_ms = 0;
unsigned long new_ms = 0;
volatile int n; // counter variable


////////////////// BLYNK CODE ///////////////////
BLYNK_WRITE(V10) {
  
  ///////////// JOYSTICK INPUTS ///////////////////////
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0
}


// SETUP FUNCTION 
void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
    
// PCA SETUP
pca.begin();
pca.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  // Set all servos to start position
  for (i=0; i < nbPCAServo; i++);{
               //PCA Commands
               Serial.print("Startup interval ");
               Serial.println(i);
               //start_ms = millis();
               //new_ms = start_ms;
               //while (new_ms - start_ms <= interval == true) {
                spos = map(90, 0, 180, 544, 2420);
                pca.writeMicroseconds(i,spos); // Swing servo position write to PCA
                for (n=0; n<1000; n++);{}
                i=i+1;    
                //new_ms = millis();
               
               }
               pca.setPin(i,0,true); // Deactivate Pin i
              
  
    i=0; // Reset i to start position after sweeping through each servo. 
}



void loop() // MAIN LOOP
{
  Blynk.run(); // Consider making this an interrupt statement? It already runs as one kinda, only putting out new values when the input changes
  servomove();
}



void servomove() // SERVO LOOP
{
  ///////////// MOTION RESPONSE ////////////////
  if (spd > 20){                        //FORWARD MOTION
  Serial.print("Input Forward Speed = ");
  Serial.print(spd);
  
  /*  
  if (turn > 20){                     // RIGHT TURN
      Serial.print("Right");
      Serial.print(turn);
      Serial.println(" ");
                                        // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
         sspd = map(spd, 20, 128, 0, 100);              // Adjusted Servo Speed: linear map 0 to 100
         for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
                 
              if (swing <= 90) {                        // Split EXT Servo into step chunks
              ext= swing+45;                            // EXT for step squence
              }
              else {
              ext= 135-(swing-90);                     // EXT for step sequence  
              }

             
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
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
    else if (turn < -20){                // LEFT TURN
      Serial.print("Left");
      Serial.print(turn);
      Serial.println(" ");

                  // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
      sspd = map(spd, 20, 128, 0, 100);              // Adjusted Servo Speed: linear map 0 to 100
      for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }

         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
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
    else {                                     // STRAIGHT
    
    */
    
    //start_ms = millis();
    //new_ms = start_ms;
    //while ((new_ms - start_ms) < interval) {
      Serial.println("Straight");

                                        // SERVO CONTROL SECTION
      
         sspd = map(spd, 20, 128, 0, 60);              // Adjusted Servo Speed: linear map 0 to 60

       for (swing = 45; swing <= 135; swing += 10) { // SERVO POSITION CHANGE STEP LOOP
             
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
             for (n=0; n<1000; n++);{}
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
           
                          //PCA Commands
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
         for (n=0; n<1000; n++);{}    
         }
         for (swing = 135; swing >= 45; swing -= 10) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
                        //PCA Commands
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
         for (n=0; n<1000; n++);{}  
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
                        //PCA Commands
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
         for (n=0; n<1000; n++);{}  
             }

             
         }
    //     new_ms=millis();
    //}
    }
  
 
  
  else if (spd < -20) {                   // BACKWARD MOTION
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
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
          
          swing = 90; 
                         //PCA Commands
               spos = map(swing, 0, 180, 544, 2420);
               epos = map(ext,   0, 180, 544, 2420);
               pca.writeMicroseconds(0,spos); // Swing servo position write to PCA
               pca.writeMicroseconds(1,epos); // Extension servo position write to PCA
      n=0;
    }
    }
  
}
