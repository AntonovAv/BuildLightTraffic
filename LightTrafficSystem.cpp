// 
// 
// 

#include "LightTrafficSystem.h"

LightTrafficSystem::LightTrafficSystem(SystemState* st) {
	currentState = st;
};

void LightTrafficSystem::setCurrentState() {
	Serial.print(F("before Delete: ")); Serial.println(SystemUtils.freeRam());
	if (currentState->getNextState() != 0) {
		SystemState* newState = currentState->getNextState();
		Serial.print(F("copy: ")); Serial.println(SystemUtils.freeRam());
		delete currentState;
		Serial.print(F("delete: ")); Serial.println(SystemUtils.freeRam());
		currentState = newState;
	}
	
}

void LightTrafficSystem::process() {
	currentState->process();
	setCurrentState();
	delay(currentState->getDelayAfterProcessState());
}