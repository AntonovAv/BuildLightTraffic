// 
// 
// 

#include "ReadIdsState.h"
#include "ConnectToWiFiState.h"

ConnectToWiFiState::ConnectToWiFiState() {
	nextState = 0;
	delayMs = 1000;
	numberOfMaxRepeat = 3;
}

ConnectToWiFiState::~ConnectToWiFiState() {

}

void ConnectToWiFiState::process() {
	String responce = SystemUtils.connectToWiFi();
	Serial.println(responce);
	// if good read TODO: create error handler
	nextState = new ReadIdsState();
}

