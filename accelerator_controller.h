#include <Servo.h>

const uint8_t MIN_ACCELERATOR     = 1; // Position of actuator in millimetres
const uint8_t MAX_ACCELERATOR     = 50; // Position of actuator in millimetres

#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define ACCELERATOR_TOL 1

#define MOTOR1_MAX 660
#define MOTOR1_MIN 400


class AcceleratorController
{
  public:
    AcceleratorController(uint8_t debug);

    void setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time);
    float getCurrentPosition();
    void setup();
    void start();
    void stop();
    void run();
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
    const uint8_t actuator_pin;
    const uint8_t potentiometer_pin = 3;

    Servo actuator;
};

// Initialise the AcceleratorController
// pass true for debug to get Serial replies
AcceleratorController::AcceleratorController(uint8_t debug)
{
  this->debug = debug;
  is_moving = 0;
  //actuator.attach(actuator_pin);  // attaches the RC signal on pin 5 to the servo object 
}

void AcceleratorController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

// Return the current position of the linear actuator in millimetres
float AcceleratorController::getCurrentPosition()
{
  return analogRead(this->potentiometer_pin);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t AcceleratorController::getMovingStatus()
{
  return this->is_moving;
}

void AcceleratorController::start() {
  actuator.writeMicroseconds(FORWARDS);
}

void AcceleratorController::stop() {
  actuator.writeMicroseconds(BACKWARDS);
}

void AcceleratorController::run() {

}

// Move the linear actuator to a target position in millimetres over time in milliseconds
void AcceleratorController::setTargetPosition(Servo actuator, uint16_t target_pos, uint16_t time)
{
    this->target_value = (float)target_pos;

    float current_value = analogRead(this->potentiometer_pin);
    
    if(abs(this->target_value - current_value) > ACCELERATOR_TOL){
        this->is_moving = 1;

        if (this->target_value > current_value){// move forwards
            actuator.writeMicroseconds(FORWARDS);
            //Serial.println("[Break Controller] Moving forwards");
        }else{
            actuator.writeMicroseconds(BACKWARDS);
            //Serial.println("[Break Controller] Moving backwards");
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

        if (this->target_value > current_value){// move forwards
            this->actuator.writeMicroseconds(FORWARDS);
            //Serial.println("[Break Controller] Moving forwards");
        }else{
            this->actuator.writeMicroseconds(BACKWARDS);
            //Serial.println("[Break Controller] Moving backwards");
        }
    }
    else if (this->is_moving){
      //Serial.println("[Break Controller] Stopping");
      actuator.writeMicroseconds(STOP);
        this->is_moving = 0;
    }
  }
}
