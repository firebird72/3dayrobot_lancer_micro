#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"

IgnitionController ignitionController(true);
BrakeController    brakeController(true);
GearController     gearController(true);

void setup() {
  Serial.begin(9600);
  Serial.println("Initialising!");
}

void loop() {
    ignitionController.loop(100);
    brakeController.loop(1);
    gearController.loop(100);
}
