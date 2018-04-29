#include <Servo.h>

class ServoController
{
	public:
		ServoController(uint8_t debug, char* class_name);

		void      	log(String message);
		void 		setPin(uint8_t servo_pin);
		void 		setRange(uint16_t min_pos, uint16_t max_pos);
		void 		setDefaultPosition(uint16_t default_pos);
		uint16_t  	clipValue(uint16_t target_pos);
		void 		setTargetPosition(uint16_t target_pos);
		void      	die();
		void 		setup();
		void 		loop(uint8_t rate);

	private:
		uint8_t   debug;
		uint16_t  next_millis;

		uint16_t  target_pos;
		uint16_t  current_pos; // Optimistic, doesn't account for movement time
		
		char*    class_name = "GenericServoController";
		uint8_t  servo_pin;

		uint16_t min_pos; // Minimum position of servo in degrees
		uint16_t max_pos; // Maximum position of servo in degrees

		uint16_t default_pos;

		Servo device;
};

ServoController::ServoController(uint8_t debug, char* class_name) {
	this->debug = debug;
	this->class_name = class_name;
}

void ServoController::log(String message) {
  if (this->debug) {
    Serial.print(this->class_name);
    Serial.print(": ");
    Serial.println(message);
  }
}

void ServoController::setPin(uint8_t servo_pin) {
	this->servo_pin = servo_pin;
	this->device.attach(this->servo_pin);
}

void ServoController::setRange(uint16_t min_pos, uint16_t max_pos) {
	this->min_pos = min_pos;
	this->max_pos = max_pos;
}

void ServoController::setDefaultPosition(uint16_t default_pos) {
	this->default_pos = default_pos;
}

uint16_t ServoController::clipValue(uint16_t target_pos)
{
  if (target_pos > this->max_pos) {
    target_pos = this->max_pos;
  } else if (target_pos < this->min_pos) {
    target_pos = this->min_pos;
  }

  return target_pos;
}

void ServoController::setTargetPosition(uint16_t target_pos)
{
    this->target_pos = clipValue(target_pos);
}

void ServoController::die() {
	setTargetPosition(this->default_pos);
	this->device.write(this->target_pos);
	log("die sequence invoked");
}

void ServoController::setup() {
	log("initialised");
  	setTargetPosition(this->default_pos);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void ServoController::loop(uint8_t rate)
{
	if (millis() >= next_millis) {
    	next_millis = millis() + rate;
    	if(abs(this->target_pos != this->current_pos)) {
    		this->device.write(this->target_pos);
    		String message = "Moving to " + String(this->target_pos) + " from " + String(this->current_pos);
            log(message);
    		this->current_pos = this->target_pos;
    	}
    }
}
