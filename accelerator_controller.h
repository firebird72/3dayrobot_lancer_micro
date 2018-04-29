#include <Servo.h>

const uint8_t MIN_ACCELERATOR     = 1; // Position of actuator in millimetres
const uint8_t MAX_ACCELERATOR     = 50; // Position of actuator in millimetres
const uint8_t ACCELERATOR_TOL     = 10;

#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500

#define MOTOR1_MAX 660
#define MOTOR1_MIN 400


class AcceleratorController
{
  public:
    AcceleratorController(uint8_t debug);

    void setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time);
    uint16_t getCurrentPosition();
    void setup();
    void start();
    void stop();
    void run();
    uint16_t clipValue(uint16_t target_value);
    void loop(uint8_t rate);

    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_pos;
    uint16_t  target_value;
    uint16_t  time;

    uint8_t   is_moving;
    uint16_t  nextMillis;
    
    const char*   CLASS_NAME = "AcceleratorController";
    const uint8_t actuator_pin = 5;
    const uint8_t potentiometer_pin = 3;

    Servo actuator;
};

// Initialise the AcceleratorController
// pass true for debug to get Serial replies
AcceleratorController::AcceleratorController(uint8_t debug)
{
  this->debug = debug;
  this->is_moving = 0;

  this->actuator.attach(actuator_pin);  // attaches the RC signal on pin 5 to the servo object 
}

void AcceleratorController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the current position of the linear actuator in millimetres
uint16_t AcceleratorController::getCurrentPosition()
{
  return analogRead(this->potentiometer_pin);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t AcceleratorController::getMovingStatus()
{
  return this->is_moving;
}

// for remote control
void AcceleratorController::start() {
  actuator.writeMicroseconds(FORWARDS);
}

void AcceleratorController::stop() {
  actuator.writeMicroseconds(STOP);
}

void AcceleratorController::run() {

}

uint16_t AcceleratorController::clipValue(uint16_t target_value) {
  if (target_value > MOTOR1_MIN) {
    target_value = MOTOR1_MAX;
  } else if (target_value < MOTOR1_MIN) {
    target_value = MOTOR1_MIN;
  }

  return target_value;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds
void AcceleratorController::setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time)
{
    this->target_value = (uint16_t)target_pos;
    this->target_value = clipValue(this->target_value);

    float current_value = analogRead(this->potentiometer_pin);
    
    if(abs(this->target_value - current_value) > ACCELERATOR_TOL){
        this->is_moving = 1;
        if (this->target_value >= current_value){// move forwards
            actuator.writeMicroseconds(FORWARDS);
        } else {
            actuator.writeMicroseconds(BACKWARDS);
        }
    }
    else{
      actuator.writeMicroseconds(STOP);
        this->is_moving = 0;
    }

  delay(1000);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void AcceleratorController::loop(uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
    float current_value = analogRead(this->potentiometer_pin);

    if(abs(this->target_value - current_value) > ACCELERATOR_TOL){
        this->is_moving = 1;

        if (this->target_value >= current_value){// move forwards
            uint8_t value = map(FORWARDS, 0, 1023, 0, 179); 
            this->actuator.write(value);
        }else{
            uint8_t value = map(BACKWARDS, 0, 1023, 0, 179); 
            this->actuator.write(value);
        }
    }
    else if (this->is_moving){
      actuator.writeMicroseconds(STOP);
      this->is_moving = 0;
    }
  }
}
