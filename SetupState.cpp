// 
// 
// 

#include "SetupState.h"
#include "ResetModuleState.h"

void SetupState::process() {
	Serial.println("SetupState");
}

SystemState* SetupState::getNextState() {
	return new ResetModuleState();
}

SetupState::SetupState() {
	arr = new char[30];
}
SetupState::~SetupState() {
	Serial.println("im here");
	delete[] arr;
}