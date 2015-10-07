// 
// 
// 

#include "LightTrafficSystem.h"
#include "ReadIdsState.h"


LightTrafficSystem::LightTrafficSystem(SystemState* st, BasicLightStrategy* lstr) {
	currentState = st;
	currentLightStrategy = lstr;
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

void LightTrafficSystem::updateLightStrategy() {
	// update light strategy
	Serial.print(F("before upd lstr: ")); Serial.println(SystemUtils.freeRam());
	if (currentState->getLightStrategy() != 0) {
		delete currentLightStrategy;
		currentLightStrategy = 0;

		currentLightStrategy = currentState->getLightStrategy();
	}
	Serial.print(F("after upd lstr: ")); Serial.println(SystemUtils.freeRam());
}

void LightTrafficSystem::process() {
	currentState->process();

	updateLightStrategy();

	delay(currentState->getDelayAfterProcessState());

	setCurrentState();

	counterForBusyTime = 0; // system is alive
}

void LightTrafficSystem::lighting() {
	if (currentLightStrategy != 0) {
		currentLightStrategy->lighting(); // perform one in 1/10sec
	}
}

void LightTrafficSystem::checkAliveOfSystem() {
	if (counterForBusyTime < MAX_BUSY_TIME) {
		counterForBusyTime++;
	}
	else { 
		// system is stoped
		Serial.print(F("System is stoped")); Serial.println(SystemUtils.freeRam());
		if (currentState != 0) {
			delete currentState;
		}
		currentState = new ReadIdsState(); // go read ids
		counterForBusyTime = 0;
		//setCurrentState();
	}
}