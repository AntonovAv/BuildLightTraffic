// 
// Class allows to handle response from server that resieved by wifi module 
// 

#include "DataReader.h"

extern DataReader_ DataReader;

char DataReader_::handleNextChar(char c) {
	switch (state) {
		// find marker to read lenght of part of message
	case FIND_LEN:
		if (IPD_COM_TEMPLATE.charAt(char_count) == c) {
			char_count += 1;
			if (char_count == IPD_COM_TEMPLATE.length()) {
				char_count = 0;
				state = READ_LEN;
			}
		}
		else {
			char_count = 0;
			if (IPD_COM_TEMPLATE.charAt(char_count) == c) {
				char_count += 1;
			}
		}
		break;
		// read lenght of part of message and parse to int
	case READ_LEN:
		if (c == ':') {
			data_len = data_len_not_parse.toInt();
			data_len_not_parse = "";
			state = READ_DATA;
		}
		else {
			data_len_not_parse += c;
		}
		break;
		// read raw data from module (max 1460 chars)
	case READ_DATA:
		data_len -= 1; // read next char
		if (-1 < data_len) {
			/***************************************************/
			if (false == isReadHeader) { // if not read header
				responseHeader += c;
				if (END_OF_HEADER_TEMPLATE.charAt(char_count) == c) {
					char_count += 1;
					if (char_count == END_OF_HEADER_TEMPLATE.length()) {
						char_count = 0;
						isReadHeader = true;
						if (responseHeader.indexOf(CHUNCKED_TEMPLATE) != -1) {
							isChunked = true;
						}
					}
				}
				else {
					char_count = 0;
					if (END_OF_HEADER_TEMPLATE.charAt(char_count) == c) {
						char_count += 1;
					}
				}
			}
			else {
				if (true == isChunked) { // if data encoding is chuncked 
					chunked_data_len -= 1; // read next chuncked char
					if (-1 < chunked_data_len) {
						return c; // raw data without chuncked
					}
					else {  // read chuncked data  len
						char_count += 1; // skip first \r\n characters
						if (char_count > 2) {
							if (c == '\n') {
								chunked_data_len_not_parse.trim();// delete \r\n characters 
								chunked_data_len = hexToDec(chunked_data_len_not_parse); // convert lenght to int
								chunked_data_len_not_parse = "";
							}
							else {
								chunked_data_len_not_parse += c;
							}
						}
					}
				}
				else {
					return c; // data if encoding is not chuncked
				}
			}
			/*************************************************/
		}
		else {
			state = FIND_LEN;
		}
		break;
	}
	return -1;
}

unsigned int DataReader_::hexToDec(String hexString) {

	unsigned int decValue = 0;
	int nextInt;

	for (int i = 0; i < hexString.length(); i++) {

		nextInt = int(hexString.charAt(i));
		if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
		if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
		if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
		nextInt = constrain(nextInt, 0, 15);

		decValue = (decValue * 16) + nextInt;
	}

	return decValue;
}

void DataReader_::initRead(void) {
	char_count = 0;
	isReadHeader = false;
	isChunked = false;
	state = FIND_LEN;
	data_len = 0;
	chunked_data_len = 0;
	responseHeader = "";
}


String DataReader_::getLastHeader() {
	return responseHeader;
}
