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
	LightTrafficSystem(SystemState* state);

	void process();
	void setCurrentState();
private:
	SystemState* currentState;
};

#endif

