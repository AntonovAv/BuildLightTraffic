// 
// 
// 

#include "eeprom.h"
#include "ReadIdsState.h"
#include "ReadDataOfIdsState.h"
#include "BuildServerErrorState.h"
#include "BuildsFailedAndRunningLightStrategy.h"
#include "BuildsFailedLightStrategy.h"
#include "BuildsSuccessLightStrategy.h"
#include "BuildServerRequestErrorLightStrategy.h"
#include "DataReader.h"
#include "JSONDataParser.h"


ReadDataOfIdsState::ReadDataOfIdsState() {
	delayMs = 5000; // 5 sec default
	//delayMs = 0;
	MAX_REPEATS = 6;

	countOfRepeats = 0;

	nextState = 0;

	lightStrategy = 0;
}

ReadDataOfIdsState::~ReadDataOfIdsState() {

}

void ReadDataOfIdsState::process() {
	
	STATE_OF_BUILDS = SUCCESS;

	int currentAddress = SystemUtils.START_EEPROM_ADDRESS_BUILD_IDS;
	byte lenIds = EEPROM.read(currentAddress++);
	String currentID;
	byte resp;

	for (int i = 0; i < lenIds; i++) {

		currentAddress = SystemUtils.readFromEEPROMToString(currentAddress, 0, currentID);
		resp = handleID(currentID, i == 0);
		// if read ids and lost connection - try to connect again
		if (resp == NOT_CONNECTED_ERROR && i != 0) {
			resp = handleID(currentID, true);
		}
		currentID = "";

		Serial.print(F("Memory after handle: ")); Serial.println(SystemUtils.freeRam());
		
		if (resp != NO_ERRORS) {
			break;
		}
		// no need to read other ids if one is failed
		if (STATE_OF_BUILDS == FAILED) {
			break;
		}
	}

	SystemUtils.closeConnectionCommand();
	
	// choose next state
	if (resp == NO_ERRORS) {

		delayMs = 60000; // 1 minute if all good

		nextState = new ReadIdsState();

		Serial.println(STATE_OF_BUILDS);
		// change light strategy
		switch (STATE_OF_BUILDS) {

		case SUCCESS: lightStrategy = new BuildsSuccessLightStrategy(); break;

		case FAILED: lightStrategy = new BuildsFailedLightStrategy(); break;

		case FAILED_AND_RUNNING: lightStrategy = new BuildsFailedAndRunningLightStrategy(); break;

		}

	}
	else {
		if (countOfRepeats < MAX_REPEATS) {

			countOfRepeats++;
			nextState = 0;
		}
		else {

			lightStrategy = new BuildServerRequestErrorLightStrategy();
			nextState = new BuildServerErrorState(); // bs error state
		}
	}
}

byte ReadDataOfIdsState::handleID(String id, boolean needConnect) {
	//String request = "/buildinfo.jsp?id=" + id; // need to change
	String request = String(F( BUILD_TYPES_URL ));
	request += String(F( BUILD_STATE_URL ));
	request.replace(F( ID_PLACEHOLDER ), id);

	byte responce = SystemUtils.prepareGetRequest(request, needConnect);
	if (responce != NO_ERRORS) {
		return responce;
	}
	// debug
	Serial.print("<s>" + request + "</s>");
	
	Serial1.print(request);

	String tokens[2] = { F("status") , F("state") };
	byte lengths[2] = { 2, 2 };  

	DataReader_* dataReader = new DataReader_(false);
	JSONDataParser_* dataParser = new JSONDataParser_(tokens, 2, lengths);

	int time = 1500; // time for wait while data are reading
	while (time > 0) {
		while (Serial1.available() > 0) {
			char c = Serial1.read();
			c = dataReader->handleNextChar(c);
			if (-1 != c) {
				dataParser->parseNextChar(c);
			}
		}
		time -= 1;
		delay(1);
	}
	
	boolean success;
	Serial.println("id: " + id);
	if (dataParser->getLengthOfDataResults()[0] != 2 || dataParser->getLengthOfDataResults()[1] != 2) {
		Serial.println(F("err read stus"));
		success = false;
	}
	else {
		/* if first  failure - faild and finish
		   if second - failure, and current - runnig - Faild and finish and read other configs
		*/
		if ((*dataParser->getResultData()[0][0]).equalsIgnoreCase(F("FAILURE"))) {
			STATE_OF_BUILDS = FAILED;
		}
		else {
			// if faild and running
			if ((*dataParser->getResultData()[0][1]).equalsIgnoreCase(F("FAILURE"))) {
				if ((*dataParser->getResultData()[1][0]).equalsIgnoreCase(F("running"))) {
					if (STATE_OF_BUILDS != FAILED) {
						STATE_OF_BUILDS = FAILED_AND_RUNNING;
					}
				}
			}
		}
		Serial.print("status: "); Serial.print((*dataParser->getResultData()[0][0]));
		Serial.print(" state: "); Serial.println((*dataParser->getResultData()[1][0]));
		Serial.print("status: "); Serial.print((*dataParser->getResultData()[0][1]));
		Serial.print(" state: "); Serial.println((*dataParser->getResultData()[1][1]));
		success = true;
	}
	
	/*!important*/
	delete dataReader;
	delete dataParser;

	if (success) {
		return NO_ERRORS;
	}
	else {
		return READ_STATE_OF_ID_ERROR;
	}

}