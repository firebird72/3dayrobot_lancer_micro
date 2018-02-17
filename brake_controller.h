#include <Servo.h>

const uint8_t MIN_BRAKE     = 1; // Position of actuator in millimetres
const uint8_t MAX_BRAKE     = 50; // Position of actuator in millimetres

const uint8_t MAX_LENGTH_MM = 50;

#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define BREAK_TOL 1

#define MOTOR1_MAX 660
#define MOTOR1_MIN 400


class BrakeController
{
  public:
    BrakeController(uint8_t debug);

    void setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time);
    float getCurrentPosition();
    void setup();
    void loop(uint8_t rate);

    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_pos;
    uint16_t  target_percent;
    uint16_t  time;

    uint8_t   is_moving;
    uint16_t  nextMillis;
    
    const char*   CLASS_NAME = "BrakeController";
    const uint8_t actuator_pin;
    const uint8_t potentiometer_pin = 3;

    Servo actuator;
};

// Initialise the BrakeController
// pass true for debug to get Serial replies
BrakeController::BrakeController(uint8_t debug)
{
  this->debug = debug;
  is_moving = 0;
  //actuator.attach(actuator_pin);  // attaches the RC signal on pin 5 to the servo object 
}

void BrakeController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the current position of the linear actuator in millimetres
float BrakeController::getCurrentPosition()
{
  return MAX_LENGTH_MM * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                            / (MOTOR1_MAX - MOTOR1_MIN);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t BrakeController::getMovingStatus()
{
  return this->is_moving;
}

// Move the linear actuator to a target position in millimetres over time in milliseconds
void BrakeController::setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time)
{
  this->target_percent = 100 * (float)target_pos / MAX_LENGTH_MM;

    float current_percent = 100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);
    
    if(abs(this->target_percent - current_percent) > BREAK_TOL){
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

  delay(1000);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void BrakeController::loop(uint8_t rate)
{

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
    float current_percent = 100 * (analogRead(this->potentiometer_pin) - MOTOR1_MIN) 
                                    / (MOTOR1_MAX - MOTOR1_MIN);

    if(abs(this->target_percent - current_percent) > BREAK_TOL){
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
