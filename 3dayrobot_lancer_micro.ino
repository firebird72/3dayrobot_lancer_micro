//#include "ignition_controller.h"
#include "brake_controller.h"
//#include "gear_controller.h"

//IgnitionController ignitionController(true);
BrakeController    brakeController(true);
//GearController     gearController(true);

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising!");
}

void loop() {
    //ignitionController.loop(100);
    brakeController.loop(100);

    brakeController.setTargetPosition(25,0);
    //gearController.loop(100);
}
