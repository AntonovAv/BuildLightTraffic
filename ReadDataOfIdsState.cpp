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

		handleID(currentID, i == 0);
		currentID = "";
	}

	SystemUtils.closeConnectionCommand();
	nextState = new ReadIdsState();
}

boolean ReadDataOfIdsState::handleID(String id, boolean isFirst) {
	String command = SystemUtils.prepareGetRequest("/buildinfo.jsp?id=" + id, isFirst); // need to change
	//Serial.print("<s>" + command + "</s>");
	Serial1.print(command);

	String tokens[2] = { F("status") , F("state") };
	byte length[2] = { 2, 2 }; // 

	DataReader_* dataReader = new DataReader_(false);
	JSONDataParser_* dataParser = new JSONDataParser_(tokens, 2, length);

	int time = 1000; // time for wait while data are reading
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
		Serial.print("status: "); Serial.print((*dataParser->getResultData()[0][0]));
		Serial.print(" state: "); Serial.println((*dataParser->getResultData()[1][0]));
		Serial.print("status: "); Serial.print((*dataParser->getResultData()[0][1]));
		Serial.print(" state: "); Serial.println((*dataParser->getResultData()[1][1]));
		success = true;
	}
	
	/*!important*/
	delete dataReader;
	delete dataParser;

	return success;
}