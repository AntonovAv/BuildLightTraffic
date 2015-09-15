// JSONDataParser.h

#ifndef _JSONDATAPARSER_h
#define _JSONDATAPARSER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class JSONDataParser_ {
public:

	void initParser(String* tokens, int nTokens, byte* maxLengthDataSize);

	void clearMemory();

	void parseNextChar(char &c);
	int* getLengthOfDataResults();
	String*** getResultData();

private:

	void extendResultsArray(int nOfToken, byte n);

	String* tokensForFind;
	int *resultLengths;
	byte* maxDataLengths;
	int* charCounters;
	String*** results;
	int numberOfTokens;

	int nOfTokenForReadData = -1;
	String tokenEnd = "\":"; // ":
	char endOfDataChar = -1;

};

#endif

