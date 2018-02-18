#include <Servo.h>
//const uint8_t MAX_LENGTH_MM = 50;
const uint16_t GEAR_POSITIONS[] = {0,30,60,90}; // Position of actuator in millimetres

int pin_one = 7;
int pin_two = 6;

int analog_pin = 0;


class GearController
{
  public:
    GearController(uint8_t debug);

  private:

};

// Initialise the GearController
// pass true for debug to get Serial replies
GearController::GearController(uint8_t debug)
{
      pinMode(pin_one, OUTPUT);
  pinMode(pin_two, OUTPUT);


}



void goForward(){
  digitalWrite(pin_one, LOW);
  digitalWrite(pin_two, HIGH);
  
}

void goBackward(){
  digitalWrite(pin_one, HIGH);
  digitalWrite(pin_two, LOW);
}

void stopMoving(){
  digitalWrite(pin_one, HIGH);
  digitalWrite(pin_two, HIGH);
}

int readPositionRaw(){
  return analogRead(analog_pin);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void GearController::loop(Servo servo, uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code

    Serial.println(readPositionRaw());

  }
}
