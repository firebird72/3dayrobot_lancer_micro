/*
* Small belt connects a gear on the steering column to a pot.
* Read the pot output as an analog input to tell us position.
* Value range will be 0 - 1023?
* Or 0 - 5V? Position: 5/360.
* Angle at 5V / 5.
* y = mx + b >> angle = voltage * scale + offset
* get values at 0 and full lock
*/

int potPin = A0;    // input for the pot
int potValue = 0;   // Store pot value here
int wheelAngle = 0;
double deg = 5/360; // check that 5V is the upper limit

void setup(){
  // do i need to declare pin as input
}

void loop(){
sensorValue = analogRead(potPin);
// print to console
Serial.print(sensorValue);

// wheelAngle = deg * sensorValue;
}
