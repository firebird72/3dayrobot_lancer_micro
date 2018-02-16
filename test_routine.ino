/*
	A test routine to ensure all components of car are working.
*/

#include "ignition_controller.h"
#include "brake_controller.h"
#include "gear_controller.h"

IgnitionController ignitionController(true);
BrakeController    brakeController(true);
GearController     gearController(true);

#define INIT_TARGET_GEAR_POSITION
#define TEST_BRAKE_POSITION 20 // mm


void setup() {
  Serial.begin(9600);
  Serial.println("Initialising!");
}

void loop() {


	// ignition on
	println("Turning ignition on... \n");
    ignitionController.start();
    delay(5000);

    // test accelerator
    println("Testing accelerator... \n");
    gearController.setTargetGear();
    delay(5000);
    //println()

    // brakes
    println("Testing brakes ... \n");
    brakeController.setTargetPosition(TEST_BRAKE_POSITION);
    delay(5000);

    // gps
    // read coordinates
    println("Testing gps... \n");

    // imu
    println("Testing imu...\n");

    // steering 
    println("Testing steering... \n");
    println("Turn right... \n");
    // turn steering here
    println("Turn left...\n");

    // magnetometer 
    // check headings
    println("Testing magnetomer...\n");

    // ignition off
    println("Turning ignition off ...\n");
    ignitionController.stop();

    // loop forever
    println("Testing complete, loop forever...\n");
    while(1);
}
