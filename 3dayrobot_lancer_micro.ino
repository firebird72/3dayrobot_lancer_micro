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
  Serial.println("foo");
  dataParser.parseExternalData("#0123456790123456790123\n");
  
}

void loop() {
    // ignitionController.loop(100);
    // brakeController.loop(1);
    // gearController.loop(100);
}
