#include <Servo.h>
#include "config.h"
#include "data_parser.h"
#include "ignition_controller.h"
#include "actuator_relay_controller.h"
#include "servo_controller.h"

IgnitionRelayController ignitionController(false, "ignitionController");
ActuatorRelayController brakeController(false, "brakeController");
ActuatorRelayController gearController(false, "gearController");
ServoController         steeringController(false, "steeringController");
ServoController         acceleratorController(false, "acceleratorController");
DataParser         		  dataParser(true);

uint16_t no_serial_cycles = 0;
String command;

void setup() {
  pinMode(DEADMAN_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  // Configure ignitionController
  ignitionController.setPins(IGNITION_ACC_PIN,IGNITION_START_PIN);
  ignitionController.setup();

  // Configure brakeController
  brakeController.setPins(BRAKE_PUSH_PIN,BRAKE_PULL_PIN,BRAKE_READ_PIN);
  brakeController.setRange(BRAKE_MIN,BRAKE_MAX);
  brakeController.setDefaultPosition(BRAKE_DEFAULT);
  brakeController.setup();

  // Configure gearController
  gearController.setPins(GEAR_PUSH_PIN,GEAR_PULL_PIN,GEAR_READ_PIN);
  gearController.setRange(GEAR_MIN,GEAR_MAX);
  gearController.setDefaultPosition(GEAR_DEFAULT);
  gearController.setup();

  // TODO: Steering controller needs feedback mechanism from potentiometer
  // Configure steeringController 
  steeringController.setPin(STEERING_PIN);
  steeringController.setRange(STEERING_MIN,STEERING_MAX);
  steeringController.setDefaultPosition(STEERING_DEFAULT);
  steeringController.setup();

  // Configure acceleratorController
  acceleratorController.setPin(ACCELERATOR_PIN);
  acceleratorController.setRange(ACCELERATOR_MIN,ACCELERATOR_MAX);
  acceleratorController.setDefaultPosition(ACCELERATOR_DEFAULT);
  acceleratorController.setup();

  dataParser.setup();
}

void die() {
  brakeController.die();
  gearController.die();
  ignitionController.die();
  acceleratorController.die();
  steeringController.die();
}

void loop() {
  if (digitalRead(DEADMAN_PIN) == HIGH) {
    die();
  } else {
    logic();
    brakeController.loop(100);
    gearController.loop(100);
    ignitionController.loop(100);
    acceleratorController.loop(100);
    steeringController.loop(100);
  }
}

void logic() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    dataParser.parseExternalData(command);
    if (dataParser.getValidStatus() == true) {
      ignitionController.setTargetState(dataParser.getExpectedIgnitionStatus());
      brakeController.setTargetPosition(dataParser.getExpectedBrakePosition());
      gearController.setTargetPosition(dataParser.getExpectedGearPosition());
      acceleratorController.setTargetPosition(dataParser.getExpectedAcceleratorPosition());
      steeringController.setTargetPosition(dataParser.getExpectedSteeringPosition());

      no_serial_cycles = 0;
    }
  } else {
    no_serial_cycles++;
    if (no_serial_cycles == MAX_NO_SERIAL_CYCLES) {
      die();
    }
  }
}