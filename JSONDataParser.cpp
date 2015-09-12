// 
// 
// 

#include "JSONDataParser.h"


void JSONDataParser_::initParser(String *tokens, int nTokens, int maxLengthDataSize){

	numberOfTokens = nTokens;
	tokensForFind = new String[nTokens];

	resultLengths = new int[nTokens];
	charCounters = new int[nTokens];

	results = new String* [nTokens];

	// init values
	for (int i = 0; i < nTokens; i++) {
		resultLengths[i] = 0;
		charCounters[i] = 0;
		tokensForFind[i] = tokens[i] + tokenEnd;
		results[i] = new String[maxLengthDataSize];
	}
}

void JSONDataParser_::clearMemory() {
	delete[] tokensForFind;
	delete[] resultLengths;
	delete[] charCounters;
	for (int i = 0; i < numberOfTokens; i++) {
		delete[] results[i];
	}
}

void JSONDataParser_::parseNextChar(char &c) {
	for (int i = 0; i < numberOfTokens; i++) {
		if (i == nOfTokenForReadData) { // read data for current token
			if (endOfDataChar == -1) { // if not read end of data char(first char after token)
				switch (c) {
				case '"': 
					endOfDataChar = '"';
					return;
				case '{':
					endOfDataChar = '}';
					return;
				case '[':
					endOfDataChar = ']';
					return;
				default:
					endOfDataChar = ','; 
					break; // go to read char to data
				}
			}
			if (c != endOfDataChar) {
				results[i][resultLengths[i]] += c; // add char to data
			}
			else {
				resultLengths[i] += 1;
				endOfDataChar = -1;
				nOfTokenForReadData = -1;
			}
		}
		else {
			if (tokensForFind[i].charAt(charCounters[i]) == c) {
				charCounters[i] += 1;

				if (tokensForFind[i].length() == charCounters[i]) {
					nOfTokenForReadData = i; // current token ready for read data
					charCounters[i] = 0;
				}
			}
			else {
				charCounters[i] = 0;
				if (tokensForFind[i].charAt(0) == c) {
					charCounters[i] += 1;
				}
			}
		}
	}
}

int* JSONDataParser_::getLengthOfDataResults() {
	return resultLengths;
}

String**  JSONDataParser_::getResultData() {
	return results;
}