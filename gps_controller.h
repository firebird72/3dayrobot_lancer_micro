#include<Adafruit_GPS.h>
// May need to change depending on other things 
// Serial Port for GPS
#define GPSSerial Serial1
// We don really want a GPS echo 
#define GPSECHO false

class GpsController
{
	public:
		GpsController(uint8_t debug);
		void Setup();
		bool CheckFix();
		float RunLoop();
		float GetLat();
		float GetLong();
		float GetAngle();
	private:
		float GpsLat;
		float GpsLong;
		float GpsAngle;

}

GpsController::GpsController(uint8_t debug)
{
	this->debug = debug;
	if (debug) {
    	Serial.print(CLASS_NAME);
    	Serial.println(": initialised");
  }
}

GpsController::Setup()
{
	  Serial.begin(115200);
	  GPS.begin(9600);
	  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
	  GPS.sendCommand(PGCMD_ANTENNA);
	  delay(1000); // This is needed for some reason 
}

GpsController::CheckFix()
{	char c = GPS.read();
	if (GPS.fix) {
		Serial.print("GPS Fixed");
			return true;
	}
	else{
		Serial.print("Poor GPS Signal");
		return false;
	}
}

GpsController::RunLoop()
{
	char c = GPS.read()
	if (GPS.fix){
		GpsLat = GPS.lat; // This is in no way confusing 
		GpsLong = GPS.long;
		GpsAngle = GPS.angle;
	}
}

GpsController::GetLat()
{
	return GpsLat;
}

GpsController::GetLong()
{
	return GpsLong;
}

GpsController::GetAngle()
{
	return GpsAngle;
}