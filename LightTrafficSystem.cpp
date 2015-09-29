// 
// 
// 

#include "LightTrafficSystem.h"

LightTrafficSystem::LightTrafficSystem(SystemState* st) {
	currentState = st;
};

void LightTrafficSystem::setCurrentState() {
	Serial.print("after Delete: "); Serial.println(SystemUtils.freeRam());
	SystemState* newState = currentState->getNextState();
	Serial.print("copy: "); Serial.println(SystemUtils.freeRam());
	delete currentState;
	Serial.print("delete: "); Serial.println(SystemUtils.freeRam());
	currentState = newState;
}

void LightTrafficSystem::process() {
	currentState->process();
	
	setCurrentState();
}