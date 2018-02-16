const uint16_t GEAR_POSITIONS[] = {100,120,140,160,180}; // Position of actuator in millimetres

class GearController
{
  public:
    GearController(uint8_t debug);

    void setTargetGear(uint8_t target_gear);
    void loop(uint8_t rate);

    uint16_t getCurrentGear();
    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint8_t   current_gear; // this should be set by the potentiometer so if the unit is power cycled, it doesn't assume position 0 
    uint16_t  target_gear;
    uint16_t  nextMillis;
    uint16_t  currInterpolationTim
    
    const char* CLASS_NAME = "GearController";
};

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
  this->debug = debug;
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the last known gear
uint16_t GearController::getCurrentGear()
{
  return current_gear;
}

// Return whether the linear actuator is actually moving at the moment
uint8_t GearController::getMovingStatus()
{
  // serial comms
  return 0;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds based on a prededined value for target_gear
void GearController::setTargetGear(uint8_t target_gear)
{
  // using potentiometer 
  if (target_gear != current_gear) {
    current_gear == target_gear;
  }

  // go to GEAR_POSITION[target_gear]
  // if at GEAR_POSITION[target_gear] then update current_gear = target_gear and moving to false
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code

    // check for serial comms and get target gear
    uint8_t target_gear = 0;

    setTargetGear(target_gear); 

  }
}