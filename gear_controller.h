#include <Servo.h>
//const uint8_t MAX_LENGTH_MM = 50;
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

    void setTargetGear(Servo actuator, uint8_t target_gear, uint16_t time);
    void loop(uint8_t rate);

    uint16_t getCurrentGear();
    uint8_t  getMovingStatus();
    uint8_t convertPercentToGear(float percent);

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_percent;
    uint16_t current_percent;

    uint8_t   current_gear; // this should be set by the potentiometer so if the unit is power cycled, it doesn't assume position 0 
    uint16_t  target_gear;
    uint16_t  nextMillis;
    
    const char* CLASS_NAME = "GearController";
    const uint8_t actuator_pin = 2;
    const uint8_t potentiometer_pin = 0;

    Servo actuator;  // create servo object to control a RoboClaw channel

    uint8_t is_moving;
};

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
  this->debug = debug;

  is_moving = 0;

  this->actuator.attach(this->actuator_pin);  // attaches the RC signal on pin 5 to the servo object

  this->current_percent =  100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);

  this->current_gear = convertPercentToGear(this->current_percent);

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
        float dist = abs(this->current_percent - GEAR_POSITIONS[i]);
        if(dist < best_dist){
            best_dist = dist;
            best_gear = i;
        }
    }
}

// Return the last known gear
uint16_t GearController::getCurrentGear()
{

    float current_percent = 100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);
    return convertPercentToGear(current_percent);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t GearController::getMovingStatus()
{
  return this->is_moving;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds based on a prededined value for target_gear
void GearController::setTargetGear(Servo actuator, uint8_t target_gear, uint16_t time)
{
    this->target_percent = 100 * (float)GEAR_POSITIONS[target_gear] / MAX_LENGTH_MM;

    this->target_gear = target_gear;

    float current_percent = 100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);
    
    if(abs(this->target_percent - current_percent) > TOL){
        this->is_moving = 1;

        if (this->target_percent > current_percent){// move forwards
            actuator.writeMicroseconds(FORWARDS);
            Serial.println("[Break Controller] Moving forwards");
        }else{
            actuator.writeMicroseconds(BACKWARDS);
            Serial.println("[Break Controller] Moving backwards");
        }
    }
    else{
      actuator.writeMicroseconds(STOP);
        this->is_moving = 0;
    }
}
// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
    float current_percent = 100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);

    if(abs(this->target_percent - current_percent) > TOL){
        this->is_moving = 1;

        if (this->target_percent > current_percent){// move forwards
            //actuator.writeMicroseconds(FORWARDS);
            Serial.println("[Break Controller] Moving forwards");
        }else{
            //actuator.writeMicroseconds(BACKWARDS);
            Serial.println("[Break Controller] Moving backwards");
        }
    }
    else if (this->is_moving){
      Serial.println("[Break Controller] Stopping");
      actuator.writeMicroseconds(STOP);
        this->is_moving = 0;
    }

  }
}
