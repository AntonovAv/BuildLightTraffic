// 
// 
// 

#include "LightTrafficSystem.h"

LightTrafficSystem::LightTrafficSystem(SystemState* st) {
	currentState = st;
};

void LightTrafficSystem::setCurrentState() {
	
	SystemState* newState = currentState->getNextState();
	delete currentState;
	currentState = newState;
}

void LightTrafficSystem::process() {
	currentState->process();
	setCurrentState();
}