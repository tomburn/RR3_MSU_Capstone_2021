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
//#include "arduino_secrets.h" // This is the new tab, it normally contains next two lines
#define SECRET_SSID "MSU-Guest"
#define SECRET_PASS ""
char ssid[] = SECRET_SSID;        // your network SSID (name)

// BREAKOUT BOARD
#define nbPCAServo 8 // The number of servos
//Parameters
#define min_imp  150 // This is the 'minimum' pulse length count (out of 4096)
#define max_imp  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
//Objects
Adafruit_PWMServoDriver pca= Adafruit_PWMServoDriver();
////////////////////////////////////////////////////////////////////////////////////////////
// Constants
const int spos= 90; // Start position 90 deg for all legs
const int dead_fwd   = 20; // Joystick Deadzone
const int dead_back  = -20;
const int dead_left  = -30;
const int dead_right = 30;

const int swing_max = 135; // Maximum Swing Servo Position with buffer zone because while loops can exceed max positions before correcting
const int swing_min = 45;  // Minimum Swing Servo Position
const int ext_max   = 180; // Mximum Extension Servo Position
const int ext_min   = 45;  // Minimum Extension Servo Position

const int turn_swing_max = 135;
const int turn_swing_min = 45;
const int turn_ext_max = 180; 
const int turn_ext_min = 45;


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
int oswing;
int ext;
int oext;
int spos_PWM;

int spd;    // Joystick Y position var - speed input
int turn;   // Joystick X position var - turning input

int adjspd = 20;   // Servo speed Adjusted, int because map() outputs ints

long int interval = 500; // wait time in millis, will be overwritten
long int newmillis;
long int oldmillis;



BLYNK_WRITE(V10) {  // BLYNK CODE
  
  // JOYSTICK INPUTS 
  int x = param[0].asInt(); //BLYNK INPUTS FROM JOYSTICK
  int y = param[1].asInt(); 
  spd  = y-128;             // CENTER JOYSTICK AT 0,0
  turn = x-128;             // CENTER JOYSTICK AT 0,0
  //adjspd = map(spd, 0, 128, 0, 30);

}


// SETUP FUNCTION 
void setup()
{
  
  Serial.begin(9600); // Debug console
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); //Blynk.begin(auth, ssid, pass);
    
  // PCA SETUP
  pca.begin();
  pca.setOscillatorFrequency(25000000); // 250000000
  pca.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~60 Hz updates digital are 300~500???
  
  
  spos_PWM = map(spos, 0, 180, min_imp, max_imp); // Set all servos to start position
  
  for (i=0; i < nbPCAServo; i++){
               //PCA Commands
               Serial.print("Startup interval ");
               Serial.println(i);
               pca.writeMicroseconds(i,spos_PWM); // Swing servo position write to PCA
            //   for (n=0; n<interval; n++);{}  // Wait loop
            //   pca.setPin(i,0,true);          // Deactivate Pin i
            newmillis = millis();
            oldmillis=millis();
            while (newmillis-oldmillis < interval){
                newmillis=millis();
               }
  }
  // Delay
            newmillis = millis();
            oldmillis=millis();
            while (newmillis-oldmillis < interval){
               newmillis=millis();
            }
}





void servomove() // SERVO LOOP

/* Servo Numbering Convention:
 *  0 Left front swing
 *  1 Left front extension
 *  2 Right front swing
 *  3 Right front extension
 *  4 Left rear swing
 *  5 Left rear extension
 *  6 Right rear swing
 *  7 Right rear extension
 */

{

if (spd > dead_fwd){           //FORWARD MOTION
  Serial.print("Forward");  
      
if (turn > dead_right){        // RIGHT TURN
          Serial.print("Right");
          
        while (swing < turn_swing_max){                          // STEP 
        swing=swing+adjspd;                              // Driving swing function
        oswing=oswing-adjspd;                            // Opposite driving swing function
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               oext = 135-(oswing-90);
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn < dead_right){
                  break;
                  
                }
               }

                Serial.println("Step sequence 1");
                
             
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA



               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn < dead_right){
                  break;
                  }
               }
                Serial.println("Step sequence 2");
                                           
               }
              
      }

      while (swing > turn_swing_min){                         // SWING
        swing=swing-adjspd;
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               
               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn < dead_right){
                  break;
                  }
               }
                Serial.println("Swing sequence 1");
                             
               
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               ext= 135-(swing-90);                      // EXT for swing sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn < dead_right){
                  break;
                  }
               }
                Serial.println("Swing sequence 2");
                                          
               }
      }      
          }


else if (turn < dead_left){// LEFT TURN
          Serial.print("Left");
          
          while (swing < turn_swing_max){                          // STEP 
        swing=swing+adjspd;                              // Driving swing function
        oswing=oswing-adjspd;                            // Opposite driving swing function
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               oext = 135-(oswing-90);
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn > dead_left){
                  break;
                  }
               }                
                Serial.println("Step sequence 1");
                
                             
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn > dead_left){
                  break;
                  }
               } 
                Serial.println("Step sequence 2");
                                           
               }
              
      }

      while (swing > turn_swing_min){                         // SWING
        swing=swing-adjspd;
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0,extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn > dead_left){
                  break;
                  }
               }               
                Serial.println("Swing sequence 1");
                             
               
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               ext= 135-(swing-90);                      // EXT for swing sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(2, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(3, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (turn > dead_left){
                  break;
                  }
               }             
                Serial.println("Swing sequence 2");
                                          
               }
      }
      }


else {                     // STRAIGHT   
      Serial.println("Straight");

      // SERVO CONTROL SECTION

      while (swing < swing_max){                          // STEP 
        swing=swing+adjspd;                              // Driving swing function
        oswing=oswing-adjspd;                            // Opposite driving swing function
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               oext = 135-(oswing-90);
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

            
               //for (n=0; n<interval; n++);{// Delay loop
                Serial.println("Step sequence 1");
               // }             
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

 
                Serial.println("Step sequence 2");
                                          
               }
                     newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (spd < dead_fwd){
                  break;
                  }
               }        
      }

      while (swing > swing_min){                         // SWING
        swing=swing-adjspd;
               if (swing <= 90) {                        // Split EXT Servo into step chunks
               ext= swing+45;                            // EXT for step squence
               
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0, extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swingrf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA


                
                Serial.println("Swing sequence 1");
                            
               
               }
               else {
               ext= 135-(swing-90);                      // EXT for step sequence  
               
               ext= 135-(swing-90);                      // EXT for swing sequence  
               oext = oswing+45;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swinglf);         // Swing servo position write to PCA
                pca.setPWM(1, 0, extlf);           // Extension servo position write to PCA
               swingrr = map(swing, 0, 180, min_imp, max_imp);
               extrr   = map(ext,   0, 180, min_imp, max_imp);
                pca.setPWM(6, 0, swingrr);         // Swing servo position write to PCA
                pca.setPWM(7, 0,extrr);           // Extension servo position write to PCA
               swingrf = map(oswing, 0, 180, min_imp, max_imp);
               extrf   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(0, 0, swingrf);         // Swing servo position write to PCA
                pca.writeMicroseconds(1,extrf);           // Extension servo position write to PCA
               swinglr = map(oswing, 0, 180, min_imp, max_imp);
               extlr   = map(oext,   0, 180, min_imp, max_imp);
                pca.setPWM(4, 0, swinglr);         // Swing servo position write to PCA
                pca.setPWM(5, 0, extlr);           // Extension servo position write to PCA

          
                Serial.println("Swing sequence 2");
                                          
               }
               newmillis= millis();
               oldmillis=millis();
               while (newmillis-oldmillis < interval){
                newmillis=millis();
                  if (spd < dead_fwd){
                  break;
                  }
               }
      } 
      
    }
    }
 
  
  else if (spd < dead_back) {                   // BACKWARD MOTION
   Serial.print("STOP");


  
/*    if (turn > dead_right){
      Serial.print("Right");     
      }
    else if (turn < dead_left){
      Serial.print("Left");
      }
    else {
      Serial.print("Straight");     
      }*/
    }
 
  
  /*
   * else {
    //Serial.println("Stopped");      // NO MOTION
    n=n+1;
    
    if (n>100){         // No motion for a while, move legs back to neutral
      if (swing != 90)
          ext = 90;
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
               pca.writeMicroseconds(0,swinglf); // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf); // Extension servo position write to PCA
          
          swing = 90; 
               //PCA Commands
               swinglf = map(swing, 0, 180, min_imp, max_imp);
               extlf   = map(ext,   0, 180, min_imp, max_imp);
               pca.writeMicroseconds(0,swinglf); // Swing servo position write to PCA
               pca.writeMicroseconds(1,extlf); // Extension servo position write to PCA
      n=0;
    }
    }
    */
  
}




void loop() // MAIN LOOP
{
  Blynk.run();
  servomove();
}
