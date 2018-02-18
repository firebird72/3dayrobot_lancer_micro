#include <Servo.h>

#include "data_parser.h"
#include "ignition_controller.h"
#include "gear_controller.h"

#define DEBUG 1

IgnitionController    ignitionController(true);
GearController        gearController(true);
DataParser    		    dataParser(true);

uint16_t steering_position;
uint16_t brake_position;
uint16_t accelerator_position;
uint16_t gear_position;
uint8_t  autonomy_status = 0;
uint8_t  ignition_status = 0;
uint8_t  kill_status = 0;
uint8_t  debug = 1;

uint8_t  nextMillis = 0; 
uint8_t  rate = 200;

// HACKS
Servo throttle;
Servo steering;
/* 
  Timing mechanisms if we want to only allow commands after a certain
  period of time
*/
uint8_t ignition_interval = 500; //ms

String command;

void setup() {
  Serial.begin(9600);

  dataParser.setup();
  ignitionController.setup();
  gearController.setup();

  gearController.neutral();

  // HACKS
  throttle.attach(2);
  throttle.write(120);
  steering.attach(7);
  steering.write(90);
}

void loop() {
  dataParser.loop(100);
  logic();
  ignitionController.loop(100);
  gearController.loop(10);
}

void logic() {

  if (Serial.available()) {

    command = Serial.readStringUntil('\n');
    dataParser.parseExternalData(command);
    // ignition commands

    uint8_t _expected_ignition_status = dataParser.getExpectedIgnitionStatus();
    uint16_t _expected_accelerator_position = dataParser.getExpectedAcceleratorPosition();
    uint16_t _expected_steering_position = dataParser.getExpectedSteeringPosition();
    uint16_t _expected_gear_position = dataParser.getExpectedGearPosition();

    if (ignition_status != _expected_ignition_status) {
      if (_expected_ignition_status == 1) {
        ignitionController.start();
      } else {
        ignitionController.stop();
      }
      ignition_status = _expected_ignition_status;
    }

    if (accelerator_position != _expected_accelerator_position) {
      throttle.write(_expected_accelerator_position);
      accelerator_position = _expected_accelerator_position;
    }

    if (steering_position != _expected_steering_position) {
      steering.write(_expected_steering_position);
      steering_position = _expected_steering_position;
    }

    //gearController.setTargetPosition(_expected_gear_position, 0);
  
  	// writing back over serial comms
  	if (millis() >= nextMillis) {
  		nextMillis = millis + rate;

  		Serial.print("0000"); //steering position padding
  		Serial.print(brake_position); //brake position
  		Serial.print(accelerator_position); // accelerator position
  		Serial.print(gear_position); //gear position
  		Serial.print("0"); //autonomy status
  		Serial.print(ignition_status); //ignition status
  		Serial.print("0"); //kill status
  		Serial.print("0000"); //checksum
  		Serial.print("\n");

  	}

  }

}