// Message Format:
// [steering_position(4)][brake_position(4)][accelerator_position(4)][gear_position(4)][autonomy_status(1)][ignition_status(1)][kill_status(1)][checksum(4)]\n

class DataParser
{
  public:
    DataParser(uint8_t debug);

    void setup();
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

    
    const char* CLASS_NAME      = "DataParser";
    const char  MESSAGE_LENGTH  = 23;

    uint16_t checksum;
    uint16_t steering_position;
    uint16_t brake_position;
    uint16_t accelerator_position;
    uint16_t gear_position;
    uint16_t nextMillis;
    uint8_t  autonomy_status;
    uint8_t  ignition_status;
    uint8_t  kill_status;
    uint8_t  debug;

};

// Initialise the DataParser
// pass true for debug to get Serial replies
DataParser::DataParser(uint8_t debug)
{
  this->debug = debug;
}

void DataParser::setup() {
  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }
}

void DataParser::parseExternalData(String data) {
  if (data.length() == MESSAGE_LENGTH) {

      uint16_t _steering_position     = (uint16_t) data.substring(0,  4).toInt();
      uint16_t _brake_position        = (uint16_t) data.substring(4,  8).toInt();
      uint16_t _accelerator_position  = (uint16_t) data.substring(8,  12).toInt();
      uint16_t _gear_position         = (uint16_t) data.substring(12, 16).toInt();
      uint16_t _checksum              = (uint16_t) data.substring(MESSAGE_LENGTH - 4, MESSAGE_LENGTH).toInt();

      // 0 795 1023 1023 01201102961
      Serial.println("Checking checksum components.\n");
      Serial.println(_steering_position);
      Serial.println(_brake_position);
      Serial.println(_accelerator_position);
      Serial.println(_gear_position);
      Serial.println(_checksum);


      if (_steering_position + _brake_position + _accelerator_position + _gear_position == _checksum) {
        steering_position     = _steering_position;
        brake_position        = _brake_position;
        accelerator_position  = _accelerator_position;
        gear_position         = _gear_position;
        autonomy_status       = (uint8_t)  data.substring(16, 17).toInt();
        ignition_status       = (uint8_t)  data.substring(17, 18).toInt();
        kill_status           = (uint8_t)  data.substring(18, 19).toInt();
        checksum              = (uint16_t) data.substring(MESSAGE_LENGTH - 4, MESSAGE_LENGTH).toInt();
        Serial.println("correct sum\n");
        Serial.println(ignition_status);
        Serial.println("YOLO\n");
      } else {
        if (debug) {
          Serial.print(CLASS_NAME);
          Serial.print(": ERROR: parseExternalData: Invalid checksum\nData:");
          Serial.println(data);
          Serial.println(data.length());
          Serial.println(_steering_position + _brake_position + _accelerator_position + _gear_position);
          Serial.println(_checksum);
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