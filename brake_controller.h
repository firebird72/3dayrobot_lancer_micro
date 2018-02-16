const uint8_t MIN_BRAKE     = 1; // Position of actuator in millimetres
const uint8_t MAX_BRAKE     = 100; // Position of actuator in millimetres

class BrakeController
{
  public:
    BrakeController(uint8_t debug);

    void setTargetPosition(uint16_t target_pos, uint16_t time);
    void loop(uint8_t rate);

    uint16_t getCurrentPosition();
    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_pos;
    uint16_t  time;
    
    const char* CLASS_NAME = "BrakeController";
};

// Initialise the BrakeController
// pass true for debug to get Serial replies
BrakeController::BrakeController(uint8_t debug)
{
  this->debug = debug;
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the current position of the linear actuator in millimetres
uint16_t BrakeController::getCurrentPosition()
{
  return 100;
}

// Return whether the linear actuator is actually moving at the moment
uint8_t BrakeController::getMovingStatus()
{
  return 0;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds
void BrakeController::setTargetPosition(uint16_t target_pos, uint16_t time)
{
  
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void BrakeController::loop(uint8_t rate)
{
  if (millis() % rate == 0) {
    // Execute code
  }
}