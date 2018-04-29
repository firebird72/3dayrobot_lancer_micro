class IgnitionRelayController
{
	public:
		IgnitionRelayController(uint8_t debug, char* class_name);

		void log(String message);
		void setPins(uint8_t acc_pin, uint8_t start_pin);
		void setTargetState(uint16_t target_state);
		void die();
		void setup();
		void loop(uint8_t rate);

	private:
		uint8_t   debug;
		uint16_t  next_millis;
		uint16_t  state_delay = 500;

		uint16_t  target_state; //0: off, 1: on
		uint16_t  current_state;
		uint16_t  default_state = 0;
		
		char*    class_name = "GenericIgnitionRelayController";
		uint8_t  acc_pin;
		uint8_t  start_pin;
		uint8_t  read_pin;
};

IgnitionRelayController::IgnitionRelayController(uint8_t debug, char* class_name)
{
	this->debug = debug;
	this->class_name = class_name;
}

void IgnitionRelayController::log(String message)
{
  if (this->debug) {
    Serial.print(this->class_name);
    Serial.print(": ");
    Serial.println(message);
  }
}

void IgnitionRelayController::setPins(uint8_t acc_pin, uint8_t start_pin) {
	this->acc_pin = acc_pin;
	this->start_pin = start_pin;
}

void IgnitionRelayController::setTargetState(uint16_t target_state)
{
    this->target_state = target_state;
}

void IgnitionRelayController::die()
{
	setTargetState(this->default_state);
	log("die sequence invoked, awaiting loop();");
}

void IgnitionRelayController::setup()
{
	log("initialised");
	this->current_state = this->default_state;
  	setTargetState(this->default_state);
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void IgnitionRelayController::loop(uint8_t rate)
{
	if (millis() >= next_millis) {
	  next_millis = millis() + rate;

	  if(this->target_state > this->current_state) {
		String message = "Changing ignition state to " + String(this->target_state) + " from " + String(this->current_state);
        log(message);
	  	digitalWrite(this->acc_pin, HIGH);
	  	delay(this->state_delay);
	  	digitalWrite(this->start_pin, HIGH);
	  	delay(this->state_delay);
	  	digitalWrite(this->start_pin, LOW);
	  	this->current_state = this->target_state;
	  } else if (this->target_state < this->current_state){
	  	String message = "Changing ignition state to " + String(this->target_state) + " from " + String(this->current_state);
        log(message);
	  	digitalWrite(this->start_pin, HIGH);
	  	digitalWrite(this->start_pin, LOW);
	  	this->current_state = this->target_state;
	  }
	}
}