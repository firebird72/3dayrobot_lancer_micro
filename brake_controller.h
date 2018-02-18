#include <Servo.h>

const uint8_t MIN_BRAKE     = 1; // Position of actuator in millimetres
const uint8_t MAX_BRAKE     = 50; // Position of actuator in millimetres

#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define BREAK_TOL 20

#define BREAK_ON 530
#define BREAK_OFF 385

int pin_one = 5;
int pin_two = 6;

class BrakeController
{
  public:
    BrakeController(uint8_t debug);

    void setTargetPosition(uint16_t target_pos, uint16_t time);
    float getCurrentPosition();
    void setup();
    void loop(uint8_t rate);

    uint8_t  getMovingStatus();

    void breakON();
    void breakOFF();

  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_pos;
    uint16_t  time;

    uint8_t   is_moving;
    uint16_t  nextMillis;
    
    const char*   CLASS_NAME = "BrakeController";
    const uint8_t potentiometer_pin = 1;

    void goForward();
    void goBackward();
    void stopMoving();

    Servo actuator;
};


void BrakeController::goForward(){
  digitalWrite(pin_one, LOW);
  digitalWrite(pin_two, HIGH);
  
}

void BrakeController::goBackward(){
  digitalWrite(pin_one, HIGH);
  digitalWrite(pin_two, LOW);
}

void BrakeController::stopMoving(){
  digitalWrite(pin_one, HIGH);
  digitalWrite(pin_two, HIGH);
}

// Initialise the BrakeController
// pass true for debug to get Serial replies
BrakeController::BrakeController(uint8_t debug)
{
  this->debug = debug;
  is_moving = 0;


  
}

void BrakeController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }

  pinMode(pin_one, OUTPUT);
  pinMode(pin_two, OUTPUT);

  delay(100);
}

// Return the current position of the linear actuator in millimetres
float BrakeController::getCurrentPosition()
{
  return analogRead(this->potentiometer_pin);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t BrakeController::getMovingStatus()
{
  return this->is_moving;
}


void BrakeController::setTargetPosition(uint16_t target_pos, uint16_t time)
{
    this->target_pos = target_pos;

    float current_pos = analogRead(this->potentiometer_pin);
    
    if(abs(this->target_pos - current_pos) > BREAK_TOL){
        this->is_moving = 1;

        if (this->target_pos > current_pos){
              // move forwards
            this->goForward();
            Serial.println("[Break Controller] Moving forwards");
        }else{
            this->goBackward();
            Serial.println("[Break Controller] Moving backwards");
        }
    }
    else{
        this->stopMoving();
        this->is_moving = 0;
    }

  delay(100);
}

void BrakeController::breakON(){
  this->setTargetPosition(BREAK_ON,0);
  Serial.println("[Break Controller] Setting Position to BREAK_ON");
}

void BrakeController::breakOFF(){
  this->setTargetPosition(BREAK_OFF,0);
  Serial.println("[Break Controller] Setting Position to BREAK_OFF");
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void BrakeController::loop(uint8_t rate)
{

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;

    float current_pos = analogRead(this->potentiometer_pin);

    Serial.println("target");
    Serial.println(this->target_pos);
    Serial.println("current");
    Serial.println(current_pos);
    
    
    if(abs(this->target_pos - current_pos) > BREAK_TOL){
        this->is_moving = 1;

        if (this->target_pos > current_pos){
              // move forwards
            this->goForward();
            Serial.println("[Break Controller] Moving forwards");
        }else{
            this->goBackward();
            Serial.println("[Break Controller] Moving backwards");
        }
    }
    else{
        this->stopMoving();
        this->is_moving = 0;
    }
  }
}
