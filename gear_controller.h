#include <Servo.h>

class GearController
{
  public:
    GearController(uint8_t debug);

    void setup();

    void     resetGearStates();
    void     setTargetPosition(uint8_t target_gear);
    void     loop(uint8_t rate);

    //uint16_t getCurrentPosition();
    uint8_t  getMovingStatus();
    uint16_t getCurrentPosition();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint8_t   is_moving;
    uint16_t  target_value;
    uint16_t  current_value;
    uint16_t  nextMillis;
    uint16_t  tolerance;
    
    const char*   CLASS_NAME = "GearController";
    const uint8_t EXTEND_PIN = 2;
    const uint8_t CONTRACT_PIN = 4;
    const uint8_t FEEDBACK_PIN = 6;
};

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
  this->debug = debug;
  moving = false;
}

void GearController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the value of the potentiometer
uint16_t GearController::getCurrentPosition()
{
    this->current_value = analogRead(this->FEEDBACK_PIN);
    return current_value;
}

// Return whether the linear actuator is actually moving at the moment
uint8_t GearController::getMovingStatus()
{
  return this->is_moving;
}

// Set the target position for the  
void GearController::setTargetPosition(uint8_t target_gear)
{
    this->target_value = target_value;
}

void GearController::resetGearStates() {
  digitalWrite(CONTRACT_PIN, LOW);
  digitalWrite(EXTEND_PIN, LOW);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(uint8_t rate)
{
  // set both gears to low, ensure we only turn one way 
  resetGearStates();

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    getCurrentPosition();

    if(abs(this->target_value - current_value) > tolerance){
        this->is_moving = 1;

        if (this->target_value > current_value) {
            digitalWrite(this->EXTEND_PIN, HIGH);
            digitalWrite(this->CONTRACT_PIN, LOW);
        }else{
            digitalWrite(this->EXTEND_PIN, LOW);
            digitalWrite(this->CONTRACT_PIN, HIGH);
        }
    } else if (this->is_moving){
      resetGearStates();
      this->is_moving = 0;
    }

  }
}
