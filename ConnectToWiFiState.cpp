// 
// 
// 

#include "ReadIdsState.h"
#include "ResetModuleState.h"
#include "ConnectToWiFiState.h"
#include "WiFiConnectionErrorLightStrategy.h"

ConnectToWiFiState::ConnectToWiFiState() {
	nextState = 0;

	delayMs = 30000; // 30 sec default

	MAX_REPEATS = 10;// try to connect if not success -> reset module
	countOfRepeats = 0;

	lightStrategy = 0;
}

ConnectToWiFiState::~ConnectToWiFiState() {

}

void ConnectToWiFiState::process() {
	byte responce = SystemUtils.connectToWiFi();
	Serial.println(responce);
	if (responce == NO_ERRORS) {
		delayMs = 1000; // 1 sec if all good

		nextState = new ReadIdsState();
	}
	else {

		lightStrategy = new WiFiConnectionErrorLightStrategy();

		if (countOfRepeats < MAX_REPEATS) {
			nextState = 0;
		}
		else {
			nextState = new ResetModuleState();
		}
	}
}

