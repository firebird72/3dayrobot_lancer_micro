# Debug mode
To enable debug mode, pass true to the instantiation of the external class. Serial (FDTI) is used for debugging only.

# GpsController
GpsController is the class that is used for interfacing with the GPS module, it can be called and checked in a variety of ways, as outlined below. 
### GpsController::Setup()
This is used to set up the GPS module, it sets the relevant baud rates and will set the GPS to sample at 1Hz, this must be run before any other GPS function ( apart from debug )

### GpsController::CheckFix()
This will check to see that the GPS module has a fix, if it has a fix then it is able to log location data, otherwise not. Currently this will print to serial "GPS Fixed"  and return True  otherwise will print "Poor GPS Signal" and return false

### GpsController::