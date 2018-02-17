#include <Servo.h>
#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"
#include "data_parser.h"


IgnitionController ignitionController(true);
BrakeController    brakeController(true);
GearController     gearController(true);
DataParser         dataParser(true);

void setup() {
  Serial.begin(9600);
  
  dataParser.setup();
  brakeController.setup();
  gearController.setup();
  ignitionController.setup();
}

void loop() {
  dataParser.loop(100);
  brakeController.loop(100);
  gearController.loop(100);
  ignitionController.loop(100);
}
