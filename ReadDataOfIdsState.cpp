// 
// 
// 

#include "eeprom.h"
#include "ReadIdsState.h"
#include "ReadDataOfIdsState.h"
#include "DataReader.h"
#include "JSONDataParser.h"


ReadDataOfIdsState::ReadDataOfIdsState() {
	//delayMs = 1 * 60 * 1000; // 1 minute if all good
	delayMs = 1000;
	numberOfMaxRepeat = 3; // TODO: read status some times than read ids (one in 10min)
	nextState = 0;
}

ReadDataOfIdsState::~ReadDataOfIdsState() {

}

void ReadDataOfIdsState::process() {

	int currentAddress = SystemUtils.START_EEPROM_ADDRESS_BUILD_IDS;
	byte lenIds = EEPROM.read(currentAddress++);
	String currentID;
	for (int i = 0; i < lenIds; i++) {
		currentAddress = SystemUtils.readFromEEPROMToString(currentAddress, 0, currentID);
		Serial.println(currentID);
		handleID(currentID);
		currentID = "";
	}

	nextState = new ReadIdsState();
}

void ReadDataOfIdsState::handleID(String& id) {
	String command = SystemUtils.prepareGetRequest("/buildinfo.jsp?id=" + id); // need to change
	Serial.print("<send>" + command + "</send>");
	Serial1.print(command);

	String tokens[2] = { "status" , "state" };
	byte length[2] = { 2, 2 }; // 

	DataReader_ dataReader;
	dataReader.initRead(false);

	JSONDataParser_ dataParser;
	dataParser.initParser(tokens, 2, length);

	int time = 1000; // time for wait while data are reading
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

	Serial.println("id: " + id);
	Serial.print("status: "); Serial.print((*dataParser.getResultData()[0][0])); Serial.print(" state: "); Serial.println((*dataParser.getResultData()[0][1]));
	Serial.print("status: "); Serial.print((*dataParser.getResultData()[1][0])); Serial.print(" state: "); Serial.println((*dataParser.getResultData()[1][1]));
	
	/*!important*/
	//dataParser.clearMemory();
}