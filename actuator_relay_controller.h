class ActuatorRelayController
{
	public:
		ActuatorRelayController(uint8_t debug, char* class_name);

		void      	log(String message);
		void 		setPins(uint8_t push_pin, uint8_t pull_pin, uint8_t read_pin);
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
		uint16_t  current_pos;
		
		char*    class_name = "GenericActuatorRelayController";
		uint8_t  push_pin;
		uint8_t  pull_pin;
		uint8_t  read_pin;

		uint16_t min_pos;
		uint16_t max_pos;

		uint16_t default_pos;
};

ActuatorRelayController::ActuatorRelayController(uint8_t debug, char* class_name) {
	this->debug = debug;
	this->class_name = class_name;
}

void ActuatorRelayController::log(String message) {
  if (this->debug) {
    Serial.print(this->class_name);
    Serial.print(": ");
    Serial.println(message);
  }
}

void ActuatorRelayController::setPins(uint8_t push_pin, uint8_t pull_pin, uint8_t read_pin) {
	this->push_pin = push_pin;
	this->pull_pin = pull_pin;
	this->read_pin = read_pin;
}

void ActuatorRelayController::setRange(uint16_t min_pos, uint16_t max_pos) {
	this->min_pos = min_pos;
	this->max_pos = max_pos;
}

void ActuatorRelayController::setDefaultPosition(uint16_t default_pos) {
	this->default_pos = default_pos;
}

uint16_t ActuatorRelayController::clipValue(uint16_t target_pos)
{
  if (target_pos > this->max_pos) {
    target_pos = this->max_pos;
  } else if (target_pos < this->min_pos) {
    target_pos = this->min_pos;
  }

  return target_pos;
}

void ActuatorRelayController::setTargetPosition(uint16_t target_pos)
{
    this->target_pos = clipValue(target_pos);
}

void ActuatorRelayController::die() {
	setTargetPosition(this->default_pos);
	log("die sequence invoked, awaiting loop();");
}

void ActuatorRelayController::setup() {
	log("initialised");
  	setTargetPosition(this->default_pos);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void ActuatorRelayController::loop(uint8_t rate)
{
	if (millis() >= next_millis) {
	  next_millis = millis() + rate;

	  this->current_pos = analogRead(this->read_pin);

	  if(this->target_pos > this->current_pos) {
	  	digitalWrite(this->pull_pin, LOW);
	  	digitalWrite(this->push_pin, HIGH);
		String message = "Moving to " + String(this->target_pos) + " from " + String(this->current_pos);
        log(message);
	  } else if (this->target_pos < this->current_pos){
	  	digitalWrite(this->push_pin, LOW);
	  	digitalWrite(this->pull_pin, HIGH);
		String message = "Moving to " + String(this->target_pos) + " from " + String(this->current_pos);
        log(message);
	  }
	}
}
