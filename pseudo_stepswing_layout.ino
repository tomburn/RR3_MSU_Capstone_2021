float hip; // Full leg rotation about hip
float ext; // Extension motion
float interval = 2000 // initial 2 second wait, will be written over

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
if y>0 { //if y is in the forward position
  // change delay time to allow for full swing-step
  currentmillis=millis;
  
  spd=(y-128)/128; // basically percent speed
  for hip -45, 45, =+spd{
  print hip // display swing movement
  ext=hip+90 // idk offset extension motion somehow
  }
  for hip 45, -45, =-spd {
  print hip // display step movement
  ext = hip+90 // offset extension motion
  }
  }
  
  
}
