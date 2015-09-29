// 
// 
// 
#include "ConnectToWiFiState.h"
#include "ResetModuleState.h"

ResetModuleState::ResetModuleState() {
	nextState = new ConnectToWiFiState();
	delayMs = 1000;
	numberOfMaxRepeat = 0;
}
ResetModuleState::~ResetModuleState() {
	
}

void ResetModuleState::process() {
	Serial.println("ResetModuleState");

	digitalWrite(SystemUtils.ESP_RESET_PIN, LOW);
	delay(100);
	digitalWrite(SystemUtils.ESP_RESET_PIN, HIGH);
	delay(2000);
	Serial.print(Serial1.readString());
}
