// SystemState.h

#ifndef _SYSTEMSTATE_h
#define _SYSTEMSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SystemState { // basic class for system 

public:

	virtual ~SystemState() ;
	virtual void process() = 0;
	virtual SystemState* getNextState() = 0;

};
#endif

