// 
// 
// 

#include "BasicLightStrategy.h"


void BasicLightStrategy::leds(boolean r, boolean y, boolean g) {
	if (r) {
		analogWrite(RED_PIN, DUTY_RED);
	}
	else {
		analogWrite(RED_PIN, 0);
	}
	if (y) {
		analogWrite(YELLOW_PIN, DUTY_YELLOW);
	}
	else {
		analogWrite(YELLOW_PIN, 0);
	}
	if (g) {
		analogWrite(GREEN_PIN, DUTY_GREEN);
	}
	else {
		analogWrite(GREEN_PIN, 0);
	}
	
}