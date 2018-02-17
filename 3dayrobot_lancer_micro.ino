#include <Servo.h>
#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"
#include "data_parser.h"
#include "accelerator_controller.h"

#define DEBUG 1

IgnitionController 		ignitionController(true);
BrakeController    		brakeController(false);
GearController     	  	gearController(false);
AcceleratorController 	acceleratorController(false);
DataParser         		dataParser(false);

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
  //Serial.print("Outisde of logic loop.\n");
  if (Serial.available()) {
  	//Serial.println("Begin logic\n");
    command = Serial.readStringUntil('\n');
    dataParser.parseExternalData(command);

    uint16_t expected_ignition_status = dataParser.getExpectedIgnitionStatus();
    ignition_status = ignitionController.getCurrentStatus();

    if (ignition_status == 0 && expected_ignition_status == 1) {
        ignitionController.start();
        if (DEBUG) Serial.println("Ignit on");
        delay(1000);
        //ignitionController.run();
    } else if (ignition_status == 1 && expected_ignition_status == 1) {
    	ignitionController.run();
    	if (DEBUG) Serial.println("Ignit run");
    } else if (ignition_status == 1 && expected_ignition_status == 0) {
    	if (DEBUG) Serial.println("Ignit stop");
    	ignitionController.stop();
    }

    uint16_t expected_brake_status = dataParser.getExpectedBrakePosition();
    brake_position = brakeController.getCurrentPosition();

    if (brake_position == 0 && expected_brake_status == 1) {
      	if (DEBUG) Serial.println("Brake on");
        brakeController.start();
    } else if (brake_position == 1 && expected_brake_status == 1) {
    	if (DEBUG) Serial.println("Brake run");
    	brakeController.run();
    } else if (brake_position == 1 && expected_brake_status == 0) {
    	if (DEBUG) Serial.println("Brake on");
    	brakeController.stop();
    }

    uint16_t expected_accelerator_status = dataParser.getExpectedAcceleratorPosition();
    accelerator_position = acceleratorController.getCurrentPosition();

    if (accelerator_position == 0 && expected_accelerator_status == 1) {
        acceleratorController.start();
        if (DEBUG) Serial.println("Accel on");
    } else if (accelerator_position == 1 && expected_accelerator_status == 1) {
    	if (DEBUG) Serial.println("Accel run");
    	acceleratorController.run();
    } else if (accelerator_position == 1 && expected_accelerator_status == 0) {
    	if (DEBUG) Serial.println("Accel stop");
    	acceleratorController.stop();
    }
  
  	// writing back over serial comms
  	if (millis() >= nextMillis) {
  		nextMillis = millis + rate;

  		Serial.print("0000"); //steering position padding
  		Serial.print(brake_position); //brake position
  		Serial.print(accelerator_position); // accelerator position
  		Serial.print("0000"); //gear position
  		Serial.print("0"); //autonomy status
  		Serial.print(ignition_status); //ignition status
  		Serial.print("0"); //kill status
  		Serial.print("0000"); //checksum
  		Serial.print("\n");

  	}

  }

}