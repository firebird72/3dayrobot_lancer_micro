// Full pinout
const uint8_t  DEADMAN_PIN = 48;

const uint8_t  IGNITION_ACC_PIN = 50;
const uint8_t  IGNITION_START_PIN = 52;

// BRAKE: relay controlled linear actuator with potentiometer feedback
const uint8_t   BRAKE_PUSH_PIN = 2;
const uint8_t   BRAKE_PULL_PIN = 4;
const uint8_t   BRAKE_READ_PIN = A0;
const uint16_t  BRAKE_MIN = 0;
const uint16_t  BRAKE_MAX = 1023;
const uint16_t  BRAKE_DEFAULT = 0;

// GEAR: relay controlled linear actuator with potentiometer feedback
// R: 312, D: 457, N: 367
const uint8_t   GEAR_PUSH_PIN = 8;
const uint8_t   GEAR_PULL_PIN = 10;
const uint8_t   GEAR_READ_PIN = A1;
const uint16_t  GEAR_MIN = 312;
const uint16_t  GEAR_MAX = 457;
const uint16_t  GEAR_DEFAULT = 367;

// STEERING: PWM controlled, uses Servo lib
const uint8_t   STEERING_PIN = 12;
const uint16_t  STEERING_MIN = 0;
const uint16_t  STEERING_MAX = 270;
const uint16_t  STEERING_DEFAULT = 120;

// ACCELERATOR: PWM controlled, uses Servo lib
const uint8_t   ACCELERATOR_PIN = 14;
const uint16_t  ACCELERATOR_MIN = 120;
const uint16_t  ACCELERATOR_MAX = 220;
const uint16_t  ACCELERATOR_DEFAULT = 120;