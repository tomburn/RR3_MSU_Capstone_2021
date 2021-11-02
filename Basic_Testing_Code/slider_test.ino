#define BLYNK_TEMPLATE_ID "TMPLR_TV2ZqB"
//#define BLYNK_PRINT Serial
#define BLYNK_DEVICE_NAME "servoposition"
#define BLYNK_AUTH_TOKEN "x5Zz1cXycRID5c-svrTbuJH2nYknuuSM";
//#include <Blynk.h>
#include <SPI.h>
#include <WiFiNINA.h>
//#include <WiFiLink.h>
#include <BlynkSimpleWiFiLink.h>
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
#include <Servo.h>


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MSU-Guest";
char pass[] = "";



Servo testservo; // create servo object
BLYNK_WRITE(V1)
{
  testservo.write(param.asInt());
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Define pins that will be used, ports with ~ are PWM outputs
  testservo.attach(9); // check port number, attaches servo on pin 9 to servo object
  testservo.write(90); // set to midpt
}

void loop()
{
  Blynk.run();
}

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

 
