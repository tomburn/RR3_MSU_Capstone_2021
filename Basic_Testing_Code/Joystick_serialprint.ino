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
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino MKR 1010
  to connect your project to Blynk.

  Note: This requires WiFiNINA library
    from http://librarymanager/all#WiFiNINA

  Feel free to apply it to any other example. It's simple!
 *************************************************************/


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

Servo testservo; // create servo object
int pos = 0; // create variable position for servo position
SimpleTimer timer; // create simple timer object

BLYNK_WRITE(V10) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  Serial.print("X= "); // Prints to monitor
  Serial.print(x); 
  Serial.print("Y= ");
  Serial.println(y); // Prints to plotter  
  Serial.print(" "); // needed for second variable to print to plotter
  Serial.println(x); // Prints to plotter
  pos = x/100;
  if (pos !=0){
  timer.setInterval(2000, moveservo); // move servo after 2 seconds
  }
 }

void moveservo(){
  testservo.write(pos);
}



void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  testservo.attach(9);

}


void loop()
{
  Blynk.run();
}

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

 
