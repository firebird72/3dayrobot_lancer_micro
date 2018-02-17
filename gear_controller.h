#include <Servo.h>
//const uint8_t MAX_LENGTH_MM = 50;
// what do each of the gears mark?
const uint16_t GEAR_POSITIONS[] = {0,30,60,90}; // Position of actuator in millimetres


#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define TOL 10

#define MOTOR1_MAX 660
#define MOTOR1_MIN 400


class GearController
{
  public:
    GearController(uint8_t debug);

    void setup();

    void setTargetGear(uint8_t target_gear, uint16_t time);
    void resetGearStates();
    void loop(uint8_t rate);

    uint16_t getCurrentGear();
    uint8_t  getMovingStatus();
    uint8_t convertPercentToGear(float percent);

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_value;
    uint16_t  current_value;

    uint8_t   current_gear; // this should be set by the potentiometer so if the unit is power cycled, it doesn't assume position 0 
    uint16_t  target_gear;
    uint16_t  nextMillis;
    
    const char* CLASS_NAME = "GearController";

    // change pins
    const uint8_t relay_pin_1 = 2;
    const uint8_t relay_pin_2 = 4;
    const uint8_t potentiometer_pin = 6;

    Servo actuator;  // create servo object to control a RoboClaw channel

    uint8_t is_moving;
};

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
  this->debug = debug;

  is_moving = 0;

  this->current_value = analogRead(this->potentiometer_pin);
  this->current_gear = convertPercentToGear(this->current_value);
}

void GearController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

uint8_t GearController::convertPercentToGear(float percent){

    float best_dist = 1000;
    uint8_t best_gear = 0;

    for(int i; i < 4; i++){
        float dist = abs(this->current_value - GEAR_POSITIONS[i]);
        if(dist < best_dist){
            best_dist = dist;
            best_gear = i;
        }
    }
}

// Return the last known gear
uint16_t GearController::getCurrentGear()
{
    float current_value = analogRead(this->potentiometer_pin);
    return convertPercentToGear(current_value);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t GearController::getMovingStatus()
{
  return this->is_moving;
}

void GearController::resetGearStates() {
  digitalWrite(relay_pin_2, LOW);
  digitalWrite(relay_pin_1, LOW);
}

// Move the linear actuator to a target position in millimetres over time in milliseconds based on a prededined value for target_gear
void GearController::setTargetGear(uint8_t target_gear, uint16_t time)
{
    this->target_value = (float)GEAR_POSITIONS[target_gear];
    this->target_gear = target_gear;
    float current_value = analogRead(this->potentiometer_pin);
    
    if(abs(this->target_value - current_value) > TOL){
        this->is_moving = 1;

        if (this->target_value > current_value){// move forwards
            digitalWrite(this->relay_pin_1, HIGH);
            digitalWrite(this->relay_pin_2, LOW);
        }else{
            digitalWrite(this->relay_pin_1, LOW);
            digitalWrite(this->relay_pin_2, HIGH);
        }
    }
    else {
        //Serial.println("[Gear Controller] Stopping");
        resetGearStates();
        this->is_moving = 0;
    }
}
// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(uint8_t rate)
{
  // set both gears to low, ensure we only turn one way 
  resetGearStates();

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    
    // get current pos of the gear
    float current_value = analogRead(this->potentiometer_pin);

    if(abs(this->target_value - current_value) > TOL){
        this->is_moving = 1;

        if (this->target_value > current_value){// move forwards
            digitalWrite(this->relay_pin_1, HIGH);
            digitalWrite(this->relay_pin_2, LOW);
        }else{
            digitalWrite(this->relay_pin_1, LOW);
            digitalWrite(this->relay_pin_2, HIGH);
        }
    } else if (this->is_moving){
      //Serial.println("[Gear Controller] Stopping");
      resetGearStates();
      this->is_moving = 0;
    }

  }
}
