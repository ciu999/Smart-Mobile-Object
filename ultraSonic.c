#include "ultraSonic.h"

float getDistance(int trigger, int echo) {
	float start, end;
	digitalWrite(echo, 0);
	digitalWrite(trigger, 1);
	delayMicroseconds(10);
	digitalWrite(trigger, 0);
	start = micros();
	end = micros();
	return (end - start) / 58;
}
