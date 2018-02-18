#include <Servo.h>

class ThrottleController {
	public:
		ThrottleController(uint8_t debug);

		void setup();
		void loop(uint8_t debug);
		void setTargetPosition(uint16_t target_position);

	private:

		uint8_t debug;
		const uint8_t SERVO_PIN = 2;
		const char*   CLASS_NAME = "ThrottleController";

		Servo throttle;
};

ThrottleController::ThrottleController(uint8_t debug) {
	this->debug = debug;
	throttle.attach(SERVO_PIN);
}

void ThrottleController::setup() {
	if (debug) {
	    Serial.print(CLASS_NAME);
	    Serial.println(": initialised");
  	}
}
 
void ThrottleController::setTargetPosition(uint16_t target_position) {
	Serial.println("setTargetPosition");
	Serial.println(target_position);
	throttle.write(target_position);

}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void ThrottleController::loop(uint8_t rate)
{
  
}
