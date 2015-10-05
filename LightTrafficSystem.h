// LightTrafficSystem.h

#ifndef _LIGHTTRAFFICSYSTEM_h
#define _LIGHTTRAFFICSYSTEM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SystemState.h"

class LightTrafficSystem { // class of system

public:
	LightTrafficSystem(SystemState* state, BasicLightStrategy* lStrategy);

	void process();
	void setCurrentState();
	void lighting(); // shoud perform one in 1/1000 second
	void updateLightStrategy();
private:
	SystemState* currentState;
	BasicLightStrategy* currentLightStrategy;
};

#endif

