// COMPILES W/O Issue, blynk app registers as not offline, serial output just keeps saying shit like Connecting to blynk.cloud:80

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLR_TV2ZqB"
#define BLYNK_DEVICE_NAME "servoposition"
#define BLYNK_AUTH_TOKEN "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc


/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "TMPLR_TV2ZqB"


#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <Servo.h>

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

Servo servotest; // create servo object
float pos = 0.0; // create variable position for servo position
SimpleTimer timer; // create simple timer object
float predictor; //smooth sailing var
int corrector = 1; // smooth sailing var
int interval = 20; //  millisec wait 
unsigned long prevMillis = 0;
unsigned long currMillis = 0;
int spd;
float swing; 

BLYNK_WRITE(V10) {
  int x = param[0].asInt();
  int y = param[1].asInt();

 /* Serial.print("X= "); // Prints to monitor
  Serial.print(x); 
  Serial.print("Y= ");
  Serial.println(y); // Prints to plotter  
  Serial.print(" "); // needed for second variable to print to plotter
  Serial.println(x); // Prints to plotter
*/
 
  spd = y;
  //Serial.print("pos= ");
  //Serial.println(pos);

  currMillis = millis();
  
  if (spd !=0 == true){
    if (currMillis - prevMillis >= interval == true) {
      Serial.print("new millis"); // new time
      Serial.println(currMillis); // new time 
      prevMillis=currMillis; // update time

      //Serial.print("speed= ");
      //Serial.println(spd);
      pos=(spd-128)*(180.0/256.0);
      Serial.println(pos);
      //servotest.write(pos);
        
      for (swing = 0; swing <= 90; swing += (spd * 0.1)) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          Serial.println(swing);
          } 
       for (swing = 90; swing >= 0; swing -= (spd * 0.1)) { // goes from 0 degrees to 180 degrees
          // in steps of 1 degree
          Serial.println(swing);  
          }
          
      }
    }
}




void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  servotest.attach(9, 544, 2420); // attach to pin 9 

}


void loop()
{
  Blynk.run();
   /* for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servotest.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servotest.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                      // waits 15 ms for the servo to reach the position
  
} */
}
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

 
