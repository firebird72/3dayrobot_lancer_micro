// Message Format:
// char MESSAGE_START '#'
// char message type
// char 0x2c ','
// char[9] data1
// char 0x2c ','
// char[9] data2
// char MESSAGE_END '!'

class StateController
{
  public:
    StateController(uint8_t debug);

    void parseExternalData(char* data);
    void loop(uint8_t rate);

    uint16_t getCurrentPosition();
    uint8_t  getMovingStatus();

  private:
    uint8_t   debug;
    char*     command;


    const char    MESSAGE_START         = '#';
    const char    MESSAGE_DELIMITER     = 0x21;
    const char    MESSAGE_END           = '\n';
    const char    NO_MESSAGE            = "-1";
    
    const char* CLASS_NAME = "StateController";
};

// Initialise the StateController
// pass true for debug to get Serial replies
StateController::StateController(uint8_t debug)
{
  Reset();
  this->debug = debug;
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

void SerialCommand::Reset() {
  command = "";
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void StateController::loop(uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
  }
}