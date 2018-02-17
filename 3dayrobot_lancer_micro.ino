#include <Servo.h>
#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"
#include "data_parser.h"
#include "accelerator_controller.h"


IgnitionController 		ignitionController(true);
BrakeController    		brakeController(true);
GearController     	  	gearController(true);
AcceleratorController 	acceleratorController(true);
DataParser         		dataParser(true);

uint16_t steering_position;
uint16_t brake_position;
uint16_t accelerator_position;
uint16_t gear_position;
uint8_t  autonomy_status = 0;
uint8_t  ignition_status = 0;
uint8_t  kill_status = 0;
uint8_t  debug = 1;

/* 
	Timing mechanisms if we want to only allow commands after a certain
	period of time
*/
uint8_t ignition_interval = 5000; //ms
uint16_t start_time = 0;

String command;

void setup() {
  Serial.begin(9600);

  dataParser.setup();

  acceleratorController.setup();
  brakeController.setup();
  gearController.setup();
  ignitionController.setup();
}

void loop() {
  dataParser.loop(100);
  logic();
  brakeController.loop(100);
  gearController.loop(100);
  ignitionController.loop(100);
  acceleratorController.loop(100);
}

void logic() {

  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    dataParser.parseExternalData(command);

    if (ignition_status == 0 && dataParser.getExpectedIgnitionStatus() == 1) {
        ignitionController.start();
    } else if (ignition_status == 1 && dataParser.getExpectedIgnitionStatus() == 1) {
    	ignitionController.run();
    } else if (ignition_status == 1 && dataParser.getExpectedIgnitionStatus() == 0) {
    	ignitionController.stop();
    }

      if (brake_position == 0 && dataParser.getExpectedBrakePosition() == 1) {
        brakeController.start();
    } else if (brake_position == 1 && dataParser.getExpectedBrakePosition() == 1) {
    	brakeController.run();
    } else if (brake_position == 1 && dataParser.getExpectedBrakePosition() == 0) {
    	brakeController.stop();
    }

      if (accelerator_position == 0 && dataParser.getExpectedAcceleratorPosition() == 1) {
        acceleratorController.start();
    } else if (accelerator_position == 1 && dataParser.getExpectedAcceleratorPosition() == 1) {
    	acceleratorController.run();
    } else if (accelerator_position == 1 && dataParser.getExpectedAcceleratorPosition() == 0) {
    	acceleratorController.stop();
    }
  

  }

}