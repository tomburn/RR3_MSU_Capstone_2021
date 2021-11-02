// BLYNK SETUP
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLR_TV2ZqB"
#define BLYNK_DEVICE_NAME "servoposition"
#define BLYNK_AUTH_TOKEN "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "TMPLR_TV2ZqB"

#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <Servo.h> 
#include <math.h> 

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";

// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "MSU-Guest";
char pass[] = "";

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
//////////////////////////////////////////////END BLYNK/LIBRARY SETUP



////////// VARIABLES ///////////////

float swing = 0.0; // create variable position for servo1 LEG position
float ext = 0.0; // create variable position for servo2 EXT position
//#define PI 3.14159265;

float interval = 250; // 0.25 sec wait, will be overwritten

unsigned long prevMillis = 0;
unsigned long currMillis = 0;

int spd;    // Joystick Y position var - speed input
int turn;   // Joystick X position var - turning input
float sspd; // Servo speed Adjusted


// SERVO
Servo servo9; // Servo in channel 9 - name servo9
//Servo servo9; // Servo in channel 9 - name servo9
//Servo servo9; // Servo in channel 9 - name servo9
//Servo servo9; // Servo in channel 9 - name servo9




////////////////// BLYNK CODE ///////////////////
BLYNK_WRITE(V10) {
  
  ///////////// JOYSTICK INPUTS ///////////////////////
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0

}

void servomove (spd, turn) {
 ///////////// MOTION RESPONSE ////////////////
  if (spd > 10){                        //FORWARD MOTION
  Serial.print("Forward Speed = ");
  Serial.print(spd);
  
    if (turn > 20){                     // RIGHT TURN
      Serial.print("Right");
      Serial.print(turn);
      Serial.println(" ");
                                        // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
         sspd = spd*spd*0.001;              // Adjusted Servo Speed
         for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
             Serial.print("Swing");
             Serial.println(swing);
             Serial.print("Ext");
             Serial.println(ext);
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             Serial.println(swing);
             Serial.println(ext);
         }
         prevMillis=currMillis; // update time 
      //}
    
    }
    else if (turn < -10){                // LEFT TURN
      Serial.print("Left");
      Serial.print(turn);
      Serial.println(" ");

                  // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
         sspd = spd*spd*0.001;              // Adjusted Servo Speed
         for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
             Serial.println(swing);
             Serial.println(ext);
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             Serial.println(swing);
             Serial.println(ext);
         }
         prevMillis=currMillis; // update time 
      //}
    }
    else {                                     // STRAIGHT
      Serial.print("Straight");

                                        // SERVO CONTROL SECTION
      currMillis = millis();
      //interval=(256.0/spd) * 250;     // Time length relative to 0.25 second
      //while (currMillis - prevMillis >= interval == true) {
         sspd = spd*spd*0.001;              // Adjusted Servo Speed
         for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
             Serial.println(swing);
             Serial.println(ext);
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             Serial.println(swing);
             Serial.println(ext);
         }
         prevMillis=currMillis; // update time 
      //}
    }
  }
 
  
  else if (spd < -10) {                   // BACKWARD MOTION
   Serial.print("Backward Speed = ");
  Serial.print(spd);
  Serial.println(" ");
  
    if (turn > 10){
      Serial.print("Right");
      Serial.print(turn);
      Serial.println(" ");
    }
    else if (turn < -10){
      Serial.print("Left");
      Serial.print(turn);
      Serial.println(" ");
    }
    else {
      Serial.print("Straight");
      
    }
  }
 
  
  else {
    Serial.print("Stopped");      // NO MOTION
  }
  
  
  /*if (spd >128 == true){          // Forward Step
    currMillis = millis();
    interval=(256.0/spd) * 3000;      // Time length relative to 1 second
    while (currMillis - prevMillis >= interval == true) {
      Serial.print("new millis"); // new time
      Serial.println(currMillis); // new time 
      Serial.print("speed= ");
      Serial.println(spd);
      
      
      swing=(spd)*(180.0/256.0);
      Serial.println(swing);
      prevMillis=currMillis; // update time 
      }
    }*/ 
}


void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  servo9.attach(9, 544, 2420); // attach to pin 9 

}


void loop()
{
  Blynk.run();
  servomove();
}
