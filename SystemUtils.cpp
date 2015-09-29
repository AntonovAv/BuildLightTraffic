// 
// 
// 

#include "SystemUtils.h"
#include "eeprom.h"

String SystemUtils_::execCommand(String command, int pause, boolean needReturn) {
	// debug output:
	Serial.println("<send>" + command + "</send>");
	Serial1.print(command);
	if (needReturn) {
		Serial1.setTimeout(pause);
		String response = Serial1.readString();
		// debug output:
		Serial.println("<receive>" + response + "</receive>");
		return response;
	}
}

String SystemUtils_::prepareGetRequest(String url) {
	String resp = execCommand("AT+CIPSTART=\"TCP\",\"" + SERVER_IP + "\"," + PORT + "\r\n", 5000);

	if (resp.indexOf("OK") == -1) return "<error>" + resp + "</error>";

	/*String header = "GET " + query + " HTTP/1.1\r\nHOST: " + SERVER_IP + "\r\nAccept: application/json\r\n\r\n";*/
	String header = "GET " + url + " HTTP/1.1\r\nHOST: " + SERVER_IP + "\r\n\r\n";
	resp = execCommand("AT+CIPSEND=" + String(header.length()) + "\r\n");

	if (resp.indexOf(">") == -1) return "<error2>" + resp + "</error2>";

	return header;
}

String SystemUtils_::connectToWiFi() {
	return execCommand("AT+CWJAP=\"" + WIFI_NAME + "\",\"" + WIFI_PASS + "\"\r\n", 5000 );
}

int SystemUtils_::freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

boolean SystemUtils_::testModule() {
	return execCommand(F("AT\r\n")).indexOf("OK") != -1;
}

void SystemUtils_::closeConnectionCommand() {
	Serial.println(execCommand("AT+CIPCLOSE\r\n", 2000));
}

int SystemUtils_::updateBuildsIdsInEEPROM(String** ids, byte len) {
	EEPROM.update(START_EEPROM_ADDRESS_BUILD_IDS, len); // read len of  build ids in EEPROM from 200 adress (size = 1024 - 200 = 824) 
													// delimeter between ids is 0
	int currentAddress = START_EEPROM_ADDRESS_BUILD_IDS + 1;
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < (*ids[i]).length(); j++) {
			EEPROM.update(currentAddress++, (*ids[i]).charAt(j));
		}
		EEPROM.update(currentAddress++, 0); // delimeter
	}
	return currentAddress;
}

int SystemUtils_::readFromEEPROMToString(int start, char ch, String& buff) {
	int currentAddr = start;
	char c = EEPROM.read(currentAddr++);
	while (c != ch) {
		buff += c;
		c = EEPROM.read(currentAddr++);
	}
	return currentAddr;
}

void SystemUtils_::readBuildIdsFromEEPROM() {
	byte len = EEPROM.read(START_EEPROM_ADDRESS_BUILD_IDS); // read len of build ids
	int currentAddr = START_EEPROM_ADDRESS_BUILD_IDS + 1;
	String temp;
	for (int i = 0; i < len; i++) {
		currentAddr = readFromEEPROMToString(currentAddr, 0, temp);
		Serial.println("EEPROM: " + temp);
		temp.remove(0);
		Serial.println("curAddr: " + String(currentAddr));
	}
}