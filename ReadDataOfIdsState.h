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
	// return true if read is success
	boolean handleID(String id, boolean isFirst);
};

#endif

