// Utils.h

#ifndef _SYSTEMUTILS_h
#define _SYSTEMUTILS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SystemUtils_ {

public:

	SystemUtils_(void) {};
	~SystemUtils_() {};

	String execCommand(String command, int pause = 1000, boolean needReturn = true);
	String prepareGetRequest(String url);
	boolean testModule();
	void closeConnectionCommand();
	String connectToWiFi();

	int freeRam();

	/*
	Update in EEPROM IDS of configurations(if data are not changed than write to EEPROM not performed)
	Start adress contains len of ids
	Ids separated by 0
	return addres of end ids
	*/
	int updateBuildsIdsInEEPROM(String** ids, byte len);
	/*
	Read all IDS of configurations from EEPROM to console
	*/
	void readBuildIdsFromEEPROM();
	/*
	Read data from EEPROM from current address to concrete char
	int address of terminate char
	*/
	int readFromEEPROMToString(int startAddress, char stop, String& buffer);
	


	const String SERVER_IP = /*"192.168.101.36"*/"192.168.0.101";
	const String PORT = "8080";
	const String WIFI_NAME = "internet";
	const String WIFI_PASS = "654qwerty123";

	const byte START_EEPROM_ADDRESS_BUILD_IDS = 199;
	const byte START_EEPROM_ADDRESS_ERRORS = 0;

	const byte RED_PIN = 6;
	const byte YELLOW_PIN = 5;
	const byte GREEN_PIN = 3;
	const byte ESP_RESET_PIN = 2;
};

static SystemUtils_ SystemUtils;
#endif


