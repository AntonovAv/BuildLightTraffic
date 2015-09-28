// SetupState.h

#ifndef _SETUPSTATE_h
#define _SETUPSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SystemState.h"

class SetupState : public SystemState {
	
public:
	
	SetupState();
	~SetupState();
	void process();
	//char are[11] = "ldsdsdsd";
	SystemState* getNextState();
	//String* str;

	char* arr;
};

#endif

