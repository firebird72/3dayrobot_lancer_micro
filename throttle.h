#include <time.h>

// may need to change for breaks/accel
// May need different values for accelerator and brake
#define ACCEL_MAX_SPEED_VAL 40 // max mm for accelerator pedal
#define ACCEL_MIN_SPEED_VAL 0 // min mm for accelerator pedal
#define ACCEL_SCALE 0.5 // tune for appropriate value

#define BRAKE_MAX_SPEED_VAL 40 // max mm for brake pedal
#define BRAKE_MIN_SPEED_VAL 0 // min mm for brake pedal
#define BRAKE_SCALE 0.4 // tune for appropriate value

#define ACCEL_TIME_RANGE 5000 // ms
#define BRAKE_TIME_RANGE 4000 // ms

uint16_t scaleValue(uint16_t rawValue, bool isAccelerator) {
	// 
	uint16_t targetValue = 0;

	if (isAccelerator) {
		if (rawValue > ACCEL_MAX_SPEED_VAL) {
			targetValue = ACCEL_SCALE * ACCEL_MAX_SPEED_VAL;
		} else if (rawValue < ACCEL_MIN_SPEED_VAL) {
			targetValue = ACCEL_MIN_SPEED_VAL;
		} else {
			targetValue = rawValue * ACCEL_SCALE;
		}
	} else {
		if (rawValue > BRAKE_MAX_SPEED_VAL) {
			targetValue = BRAKE_SCALE * BRAKE_MAX_SPEED_VAL;
		} else if (rawValue < BRAKE_MIN_SPEED_VAL) {
			targetValue = BRAKE_MIN_SPEED_VAL;
		} else {
			targetValue = rawValue * BRAKE_SCALE;
		}
	}

	return targetValue;
}

void interpolate(uint16_t rawValue, int currentValue, bool isAccelerator, uint16_t currentTime) {
	// calculate linear time intervals before entering function
	// handles positive values of pedals

	uint16_t target = scaleValue(rawValue);
	uint16_t difference = currentSpeed - targetSpeed;
	uint16_t interval;

	if (isAccelerator)
		interval = difference/ACCEL_TIME_RANGE;
	else
		interval = difference/BRAKE_TIME_RANGE;

	// start time at zero
	uint16_t startTime = millis();
	uint16_t targetTime = startTime + ACCELERATE_TIME_RANGE;

	// write this value 
	currentValue = currentValue + interval;

	// exit once we hav reached target
	if (currentValue >= target) {
		return;
	}
}	
