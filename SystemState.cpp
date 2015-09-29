// 
// 
// 

#include "SystemState.h"



SystemState::~SystemState() {
	Serial.println("In destructor: " +  (lightStrategy == 0));
//	if (lightStrategy != 0) {
	//	delete lightStrategy;
//	}
}