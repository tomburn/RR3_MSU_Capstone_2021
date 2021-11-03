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
char auth[] = "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
//char ssid[] = "MSU-Guest";
char pass[] = "";
#include "arduino_secrets.h"
char ssid[] = SECRET_SSID;        // your network SSID (name)
// END SETUP



// VARIABLES

float swinglf = 0.0; // create variable position for left front LEG position
float extlf = 0.0; // create variable position for left front EXT position

float swingrf = 0.0; // create variable position for right front LEG position
float extrf = 0.0; // create variable position for right front EXT position

float swinglr = 0.0; // create variable position for left rear LEG position
float extlr = 0.0; // create variable position for left rear EXT position

float swingrr = 0.0; // create variable position for right rear LEG position
float extrr = 0.0; // create variable position for right rear EXT position

int spd;    // Joystick Y position var - speed input
int turn;   // Joystick X position var - turning input
float sspd; // Servo speed Adjusted

//#define PI 3.14159265;

float interval = 250; // 0.25 sec wait, will be overwritten

unsigned long prevMillis = 0;
unsigned long currMillis = 0;
int n = 0; // counter variable

// SERVO - Arduino Connections
Servo servo9;  // Servo in channel 9 - name servo9
Servo servo10; // Servo in channel 10 - name servo10


////////////////// BLYNK CODE ///////////////////
BLYNK_WRITE(V10) {
  
  ///////////// JOYSTICK INPUTS ///////////////////////
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0
}




void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  servo9.attach(9, 544, 2420); // SWING attach to pin 9 
  servo10.attach(10, 544, 2420); // EXT attach to pin 10
  
  servo9.write(90)
  servo10.write(90)

}


void loop()
{
  Blynk.run();
  servomove();
}

void servomove()
{
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
                 
              if (swing <= 90) {                        // Split EXT Servo into step chunks
              ext= swing+45;                            // EXT for step squence
              }
              else {
              ext= 135-(swing-90);                     // EXT for step sequence  
              }
             servo9.write(swing);
             servo10.write(ext);
             
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             servo9.write(swing);
             servo10.write(ext);
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
         sspd = 1; //spd*spd*0.001;              // Adjusted Servo Speed
         for (swing = 45; swing <= 135; swing += sspd) { // SERVO POSITION CHANGE STEP LOOP
             //servo9.write(swing);                    // tell servo to go to position in variable 'swing'
             if (swing <= 90) {                        // Split EXT Servo into step chunks
             ext= swing+45;                            // EXT for step squence
             }
             else {
             ext= 135-(swing-90);                     // EXT for step sequence  
             }
             servo9.write(swing);
             servo10.write(ext);
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             servo9.write(swing);
             servo10.write(ext);
         }
         prevMillis=currMillis; // update time 
      //}
    }
    else {                                     // STRAIGHT
      Serial.println("Straight");

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
             servo9.write(swing);
             servo10.write(ext);
             
         }
         for (swing = 135; swing >= 45; swing -= sspd) { // SERVO POSITION CHANGE SWING LOOP
             //servo9.write(swing);              // tell servo to go to position in variable 'swing'
             
             if (swing >= 90) {                         // Split EXT Servo into step chunks
             ext= 90+2*(-swing-135);                    // EXT for swing squence
             }
             else {
             ext= 180+2*(swing-90);                           // EXT for swing sequence
             }
             servo9.write(swing);
             servo10.write(ext);
             Serial.println(swing);
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
    Serial.println("Stopped");      // NO MOTION
    n=n+1;
    
    if (n>100){         // No motion for a while, move legs back to neutral
      if (swing != 90)
          ext = 0;
          Servo10.write(ext)
          
          swing = 0; 
          Servo9.write(swing)
      n=0;
    }
    }
  
}
