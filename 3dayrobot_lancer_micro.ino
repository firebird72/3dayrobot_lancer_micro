#include <Servo.h>
//#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"

//IgnitionController ignitionController(true);
Servo breakActuator;
Servo gearActuator;
BrakeController    brakeController(true);
GearController     gearController(true);

void setup() {
  Serial.begin(57600);
  Serial.println("Initialising!");

  breakActuator.attach(2);
  gearActuator.attach(3);

  brakeController.setTargetPosition(breakActuator, 45,0);
  gearController.setTargetGear(gearActuator, 1,0); // 0:P, 1:R, 2:N, 3:D

  delay(100);
  
}

void loop() {
    //ignitionController.loop(100);

    brakeController.loop(breakActuator, 10);

    gearController.loop(gearActuator, 10);
    
}
