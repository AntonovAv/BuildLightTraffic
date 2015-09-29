// BasicLightStrategy.h

#ifndef _BASICLIGHTSTRATEGY_h
#define _BASICLIGHTSTRATEGY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class BasicLightStrategy {
public:
	virtual ~BasicLightStrategy() {};
	virtual void lighting() = 0;
	virtual void updateCounterIfNeed();
};

#endif

