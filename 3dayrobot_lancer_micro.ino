#include <Servo.h>
//#include "ignition_controller.h"
#include "brake_controller.h"
//#include "gear_controller.h"

//IgnitionController ignitionController(true);
Servo actuator;  // create servo object to control a RoboClaw channel
BrakeController    brakeController(true);
//GearController     gearController(true);



void setup() {
  Serial.begin(57600);
  Serial.println("Initialising!");

  actuator.attach(3);

  brakeController.setTargetPosition(actuator, 45,0);

  delay(100);
  
}

void loop() {
    //ignitionController.loop(100);

    brakeController.loop(actuator, 10);

    //gearController.loop(100);


    
}
