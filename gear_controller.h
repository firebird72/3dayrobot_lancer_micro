#include <Servo.h>

#define BACKWARDS 0
#define FORWARDS 3000
#define STOP 1500
#define GEAR_TOL 5

#define PARK 267
#define REVERSE 312
#define NEUTRAL 367
#define DRIVE 417

int first_pin = 3;
int second_pin = 4;

class GearController
{
  public:
    GearController(uint8_t debug);

    void setTargetPosition(uint16_t target_pos, uint16_t time);
    float getCurrentPosition();
    void setup();
    void loop(uint8_t rate);

    uint8_t  getMovingStatus();

    void breakON();
    void breakOFF();
    void park();
    void neutral();
    void reverse();
    void drive();
  private:
    uint8_t   debug;
    uint8_t   moving;
    uint16_t  target_pos;
    uint16_t  time;

    uint8_t   is_moving;
    uint16_t  nextMillis;
    
    const char*   CLASS_NAME = "GearController";
    const uint8_t potentiometer_pin = 0;

    void goForward();
    void goBackward();
    void stopMoving();



    Servo actuator;
};


void GearController::goForward(){
  digitalWrite(first_pin, LOW);
  digitalWrite(second_pin, HIGH);
  
}

void GearController::goBackward(){
  digitalWrite(first_pin, HIGH);
  digitalWrite(second_pin, LOW);
}

void GearController::stopMoving(){
  digitalWrite(first_pin, HIGH);
  digitalWrite(second_pin, HIGH);
}

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
  this->debug = debug;
  is_moving = 0;


  
}

void GearController::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }

  pinMode(first_pin, OUTPUT);
  pinMode(second_pin, OUTPUT);

  delay(100);
}

// Return the current position of the linear actuator in millimetres
float GearController::getCurrentPosition()
{
  return analogRead(this->potentiometer_pin);
}

// Return whether the linear actuator is actually moving at the moment
uint8_t GearController::getMovingStatus()
{
  return this->is_moving;
}


void GearController::setTargetPosition(uint16_t target_pos, uint16_t time)
{
    this->target_pos = target_pos;

    float current_pos = analogRead(this->potentiometer_pin);
    
    if(abs(this->target_pos - current_pos) > GEAR_TOL){
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

void GearController::park(){
  this->setTargetPosition(PARK,0);
  Serial.println("[Break Controller] Setting Position to PARK");
}

void GearController::reverse(){
  this->setTargetPosition(REVERSE,0);
  Serial.println("[Break Controller] Setting Position to REVERSE");
}
void GearController::neutral(){
  this->setTargetPosition(NEUTRAL,0);
  Serial.println("[Break Controller] Setting Position to NEUTRAL");
}
void GearController::drive(){
  this->setTargetPosition(DRIVE,0);
  Serial.println("[Break Controller] Setting Position to DRIVE");
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(uint8_t rate)
{

  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;

    float current_pos = analogRead(this->potentiometer_pin);

    Serial.println("target");
    Serial.println(this->target_pos);
    Serial.println("current");
    Serial.println(current_pos);
    
    
    if(abs(this->target_pos - current_pos) > GEAR_TOL){
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
