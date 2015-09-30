// 
// 
// 

#include "ReadIdsState.h"
#include "ReadDataOfIdsState.h"
#include "DataReader.h"
#include "JSONDataParser.h"

ReadIdsState::ReadIdsState() {
	delayMs = 1000;
	numberOfMaxRepeat = 4;
	nextState = 0;
}

ReadIdsState::~ReadIdsState() {

}

// read ids to eeprom 
void ReadIdsState::process() {
	Serial.println(F("ReadIds"));

	String command = SystemUtils.prepareGetRequest(F("/builds.jsp"), true); // need to change
	Serial.println("<s>" + command + "</s>");
	Serial1.print(command);

	String tokens[2] = { F("count") , F("id") };
	byte lengths[2] = { 1, 20 }; // 1 count data, max 20 ids of configuration

	DataReader_* dataReader = new DataReader_(false);
	JSONDataParser_* dataParser = new JSONDataParser_(tokens, 2, lengths);

	Serial.print(F("before read: ")); Serial.println(SystemUtils.freeRam());
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
	
	// debug
	Serial.print(F("after read: ")); Serial.println(SystemUtils.freeRam());

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
		nextState = new ReadDataOfIdsState();
	}
	else {
		nextState = 0;
	}
}

