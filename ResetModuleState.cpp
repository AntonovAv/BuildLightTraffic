// 
// 
// 
#include "ResetModuleState.h"
#include "SetupState.h"

void ResetModuleState::process() {
	Serial.println("ResetModuleState");
}

SystemState* ResetModuleState::getNextState() {
	return new SetupState();
}

ResetModuleState::ResetModuleState() {
	Serial.println("hello");
}
ResetModuleState::~ResetModuleState() {
	Serial.println("hello");
}