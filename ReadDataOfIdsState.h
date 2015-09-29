// ReadDataOfIdsState.h

#ifndef _READDATAOFIDSSTATE_h
#define _READDATAOFIDSSTATE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "SystemState.h"

class ReadDataOfIdsState : public SystemState {
public:
	ReadDataOfIdsState();
	~ReadDataOfIdsState();

	void process();
private:
	void handleID(String& id);
};

#endif

