// 
// 
// 

#include "ReadIdsState.h"
#include "ReadDataOfIdsState.h"
#include "BuildServerErrorState.h"
#include "BuildServerRequestErrorLightStrategy.h"
#include "DataReader.h"
#include "JSONDataParser.h"

ReadIdsState::ReadIdsState() {
	delayMs = 1000;//1000
	MAX_REPEATS = 3;
	nextState = 0;
	countOfRepeats = 0;

	lightStrategy = 0;
}

ReadIdsState::~ReadIdsState() {

}

// read ids to eeprom 
void ReadIdsState::process() {

	Serial.println(F("ReadIds"));

	byte resp = readIds();

	SystemUtils.closeConnectionCommand();

	if (resp == NO_ERRORS ) {

		delayMs = 5000; // if all good 

		nextState = new ReadDataOfIdsState();
	}
	else {
		if (countOfRepeats < MAX_REPEATS) {
			countOfRepeats++;
			nextState = 0;
		}
		else {
			lightStrategy = new BuildServerRequestErrorLightStrategy();
			nextState = new BuildServerErrorState(); //what?? problemWIthBS? orange
		}
		
	}
}

byte ReadIdsState::readIds() {
	
	//String request = "/builds.jsp";
	String request = String(F( BUILD_TYPES_URL ) );

	byte responce = SystemUtils.prepareGetRequest(request, true); // need to change

	if (responce != NO_ERRORS) {

		return responce;
	}
	// debug
	Serial.println("<s>" + request + "</s>");
	
	Serial1.print(request); // write to module

	String tokens[2] = { F("count") , F("id") };
	byte lengths[2] = { 1, 20 }; // 1 count data, max 20 ids of configuration

	DataReader_* dataReader = new DataReader_(false);
	JSONDataParser_* dataParser = new JSONDataParser_(tokens, 2, lengths);

	int time = 5000; // time for wait while data are reading
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


	// read "count"
	byte idsInResponce;
	boolean success = false;

	if (dataParser->getLengthOfDataResults()[0] != 0) {
		idsInResponce = dataParser->getResultData()[0][0]->toInt();
		Serial.print(F("len of ids: "));
		Serial.println(idsInResponce); // get len of ids
	}

	if (dataParser->getLengthOfDataResults()[1] != idsInResponce) {
		// error
		success = false;
	}
	else {
		SystemUtils.updateBuildsIdsInEEPROM(dataParser->getResultData()[1], dataParser->getLengthOfDataResults()[1]); // write ids to eeprom
		success = true;
	}

	SystemUtils.closeConnectionCommand();

	// !important
	delete dataParser;
	delete dataReader;

	if (success) {
		return NO_ERRORS;
	}
	else {
		return READ_CONFIG_IDS_ERROR;
	}
}

