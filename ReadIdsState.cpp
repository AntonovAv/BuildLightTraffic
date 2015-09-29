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
	Serial.println("ReadIds");

	String command = SystemUtils.prepareGetRequest(F("/builds.jsp")); // need to change
	Serial.println("<send>" + command + "</send>");
	Serial1.print(command);

	String tokens[2] = { "count" , "id" };
	byte lengths[2] = { 1, 20 }; // 1 count data, max 20 ids of configuration

	DataReader_ dataReader;
	dataReader.initRead(false);

	JSONDataParser_ dataParser;
	dataParser.initParser(tokens, 2, lengths);

	int time = 5000; // time for wait while data are reading
	while (time > 0) {
		while (Serial1.available() > 0) {
			char c = Serial1.read();
			c = dataReader.handleNextChar(c);
		
			if (-1 != c) {
				dataParser.parseNextChar(c);
			}
		}
		time -= 1;
		delay(1);
	}
	SystemUtils.closeConnectionCommand();

	// debug
	Serial.print("len of ids: ");
	Serial.println((*dataParser.getResultData()[0][0])); // get len of ids

	//SystemUtils.updateBuildsIdsInEEPROM(dataParser.getResultData()[1], dataParser.getLengthOfDataResults()[1]); // write ids to eeprom
	//!important
	dataParser.clearMemory();
	nextState = new ReadDataOfIdsState();
}

