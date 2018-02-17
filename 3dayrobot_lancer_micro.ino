#include <Servo.h>
// #include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"
#include "data_parser.h"

// IgnitionController ignitionController(true);
BrakeController    brakeController(true);
GearController     gearController(true);
DataParser         dataParser(true);

void setup() {
  Serial.begin(9600);
  brakeController.setup();
}

void loop() {
  brakeController.loop(100);
}
