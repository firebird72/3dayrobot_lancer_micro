// Message Format:
// #[steering_position(4)],[brake_position(4)],[accelerator_position(4)],[gear_position(1)],[autonomy_status(1)],[ignition_status(1)]\n

class DataParser
{
  public:
    DataParser(uint8_t debug);

    void parseExternalData(String data);
    void loop(uint8_t rate);

    uint16_t getExpectedSteeringPosition();
    uint16_t getExpectedBrakePosition();
    uint16_t getExpectedAcceleratorPosition();
    uint8_t  getExpectedGearPosition();
    uint8_t  getExpectedAutonomyStatus();
    uint8_t  getExpectedIgnitionStatus();

  private:
    uint8_t  getExpectedStatus();

    uint8_t  debug;

    const char    MESSAGE_START         = '#';
    const char    MESSAGE_END           = '\n';
    const char    MESSAGE_LENGTH        = 24;
    
    const char* CLASS_NAME = "DataParser";

    uint16_t steering_position;
    uint16_t brake_position;
    uint16_t accelerator_position;
    uint8_t  gear_position;
    uint8_t  autonomy_status;
    uint8_t  ignition_status;
    uint16_t nextMillis;

};

// Initialise the DataParser
// pass true for debug to get Serial replies
DataParser::DataParser(uint8_t debug)
{
  this->debug = debug;
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

void DataParser::parseExternalData(String data) {
  if (data.length() == 26) {
    if (data.substring(0,1) == MESSAGE_START && data.substring(MESSAGE_LENGTH - 1, 1) == MESSAGE_END) {
      steering_position     = (uint16_t) data.substring(0,  3).toInt();
      brake_position        = (uint16_t) data.substring(4,  8).toInt();
      accelerator_position  = (uint16_t) data.substring(8,  12).toInt();
      gear_position         = (uint8_t)  data.substring(12, 16).toInt();
      autonomy_status       = (uint8_t)  data.substring(16, 20).toInt();
      ignition_status       = (uint8_t)  data.substring(20, 24).toInt();
    } else {
      if (debug) {
        Serial.print(CLASS_NAME);
        Serial.println(": ERROR: parseExternalData: Invalid MESSAGE_START or MESSAGE_END");
      }
    }
  } else {
    if (debug) {
      Serial.print(CLASS_NAME);
      Serial.println(": ERROR: parseExternalData: Invalid MESSAGE_LENGTH");
    }
  }
}

uint16_t DataParser::getExpectedSteeringPosition() {
  return steering_position;
}

uint16_t DataParser::getExpectedBrakePosition() {
  return brake_position;
}

uint16_t DataParser::getExpectedAcceleratorPosition() {
  return accelerator_position;
}

uint8_t  DataParser::getExpectedGearPosition() {
  return gear_position;
}

uint8_t  DataParser::getExpectedAutonomyStatus() {
  return autonomy_status;
}

uint8_t  DataParser::getExpectedIgnitionStatus() {
  return ignition_status;
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void DataParser::loop(uint8_t rate)
{
  if (millis() >= nextMillis) {
    nextMillis = millis() + rate;
    // Execute code
  }
}