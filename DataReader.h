// DataReader.h

#ifndef _DATAREADER_h
#define _DATAREADER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DataReader_ {
public:
	void initRead();
	// handle next char (return -1 if char is not data or char if it is data)
	char handleNextChar(char c);
	String getLastHeader();

private:

	unsigned int hexToDec(String hexString);

	String responseHeader;

	byte state;

	static const byte FIND_LEN = 0;
	static const byte READ_LEN = 1;
	static const byte READ_DATA = 2;
	static const byte READ_HEADER = 3;

	boolean isReadHeader = false; // if header already read
	boolean isChunked = false;  // if data encoding is chuncked

	String CHUNCKED_TEMPLATE = "chunked";
	String IPD_COM_TEMPLATE = "+IPD,";
	String END_OF_HEADER_TEMPLATE = "GMT\r\n";

	int char_count; // temp counter when need to find string

	String data_len_not_parse; // len of data after IPD
	int data_len;
	String chunked_data_len_not_parse; // len of data if chuncked encoding
	int chunked_data_len;
};

#endif