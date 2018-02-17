// bias to the right

class SteeringController {
	public:
		SteeringController(uint8_t debug);

		void setup();
		void loop(uint8_t debug);
		void setTargetPosition();
	private:
		uint8_t debug:
		uint8_t isMoving;

		uint16_t nextMillis;
}

SteeringController::SteeringController() {
	this->debug = debug;
	this->nextMillis = 0;
}

void SteeringController::setup() {
	if (debug) {
	    Serial.print(CLASS_NAME);
	    Serial.println(": initialised");
  	}
}
 

void SteeringController::loop(uint8_t rate) {

	if (millis() > nextMillis) {
		nextMillis = millis() = rate;

	}
}
/*

void SteeringController::setTargetPosition() {

}

uint8_t SteeringController::getCurrentPosition() {
	return analog
}


*/

