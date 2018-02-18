#include <Servo.h>

class ThrottleController
{
  public:
    ThrottleController(uint8_t debug);

    void setTargetAngle(Servo actuator, uint16_t target_pos, uint16_t time);
    float getCurrentPosition();
    void loop(Servo actuator, uint8_t rate);

    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    uint16_t  target_angle;
    uint16_t  time;
    uint8_t actuator_pin;

    uint8_t is_moving;
    uint16_t  nextMillis;
    
    const char* CLASS_NAME = "ThrottleController";
};

// Initialise the ThrottleController
// pass true for debug to get Serial replies
ThrottleController::ThrottleController(uint8_t debug)
{
  this->debug = debug;
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }

  is_moving = 0;

  this->potentiometer_pin = 0;

}

// Return the current position of the linear actuator in millimetres
float ThrottleController::getCurrentPosition()
{
  return this->target_angle;
}

// Return whether the linear actuator is actually moving at the moment
uint8_t ThrottleController::getMovingStatus()
{
  return this->is_moving;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds
void ThrottleController::setTargetPosition(Servo actuator, uint16_t target_angle, uint16_t time)
{
  this->target_angle = target_angle

    actuator.write(this->target_angle);
    delay(15)
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void ThrottleController::loop(Servo actuator, uint8_t rate)
{

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
    
    

  }
}
