#include <Servo.h>

#include "data_parser.h"
#include "ignition_controller.h"
#include "throttle_controller.h"

#define DEBUG 1

IgnitionController    ignitionController(true);
ThrottleController    throttleController(true);
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
  throttleController.setup();
}

void loop() {
  dataParser.loop(100);
  logic();
  ignitionController.loop(100);
  throttleController.loop(100);
}

void logic() {
  //Serial.print("Outisde of logic loop.\n");
  if (Serial.available()) {
  	//Serial.println("Begin logic\n");
    command = Serial.readStringUntil('\n');
    dataParser.parseExternalData(command);
    // ignition commands

    uint8_t _expected_ignition_status = dataParser.getExpectedIgnitionStatus();
    if (ignition_status != _expected_ignition_status) {
      if (_expected_ignition_status == 1) {
        ignitionController.start();
      } else {
        ignitionController.stop();
      }
      ignition_status = _expected_ignition_status;
    }
  
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