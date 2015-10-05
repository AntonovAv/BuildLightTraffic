// 
// 
// 

#include "BuildServerErrorState.h"
#include "ResetModuleState.h"
#include "BuildServerErrorLightStrategy.h"
#include "ReadIdsState.h"

BuildServerErrorState::BuildServerErrorState() {
	MAX_REPEATS = 10;
	delayMs = 2000; //2 sec if all good
	countOfRepeats = 0;
	nextState = 0;

	lightStrategy = 0;
}

BuildServerErrorState::~BuildServerErrorState() {

}

void BuildServerErrorState::process() {
	String request = "/";
	byte resp = SystemUtils.prepareGetRequest(request, true);
	
	// wait while responce is reading
	Serial1.print(request);
	Serial1.setTimeout(2000);
	Serial1.readString();

	SystemUtils.closeConnectionCommand();

	if (resp == NO_ERRORS) {
		nextState = new ReadIdsState();
	}
	else {
		if (countOfRepeats < MAX_REPEATS) {
			delayMs = 5000; // 5 sec

			countOfRepeats++;
			nextState = 0;
		}
		else {
			delayMs = 2000; // 2 sec

			lightStrategy = new BuildServerErrorLightStrategy();

			nextState = new ResetModuleState();
		}
	}
}
