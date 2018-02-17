#include <Servo.h> 
 
Servo accel;  

int position = 0;  

 
void setup() 
{ 
  accel.attach(8);  // attach servo on pin 8
} 
 
void loop() 
{ 
  position = 0; // get from somewhere
  accel = map(val, 0, 1023, 0, 179);  // scale
  accel.write(val); // move servo to that point
} 
