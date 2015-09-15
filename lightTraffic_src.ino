#include "JSONDataParser.h"
#include "DataReader.h"
#include <TimerOne.h>

#define RED 6
#define YELLOW 5
#define GREEN 3

#define RED_MASK 1
#define RED_BLINK_MASK 2
#define YELLOW_MASK 4
#define YELLOW_BLINK_MASK 8
#define GREEN_MASK 16
#define GREEN_BLINK_MASK 32
#define BLINK_MASK 42

#define ESP_RESET 2

const String SERVER_IP = /*"192.168.101.36"*/"192.168.0.101";
const String PORT = "8088";
const String WIFI_NAME = "internet";
const String WIFI_PASS = "654qwerty123";

DataReader_ DataReader; // instanse of data reader
JSONDataParser_ DataParser;

void esp_reset() {
  digitalWrite(ESP_RESET, LOW);
  delay(100);
  digitalWrite(ESP_RESET, HIGH);
  delay(2000);
  Serial.print(Serial1.readString());
}

String exec(String command, int pause = 1000, boolean needReturn = true) {
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
  
void exec_big_data( String command ) {
	// debug output:
	Serial.println("<send>" + command + "</send>");
	Serial1.print(command);
	Serial.println("Before init reader: ");
	Serial.println(freeRam());

	DataReader.initRead(false);

	Serial.println("After init reader: ");
	Serial.println(freeRam());
	Serial.println("Before init parser: ");
	String tokens[2] = { "count", "id" };
	byte length[2] = { 1, 20 };
	Serial.println(freeRam());
	DataParser.initParser(tokens, 2, length);
	Serial.println("After init parser: ");
	Serial.println(freeRam());
	int time = 5000;
	while (time > 0) {
		while (Serial1.available() > 0) {
			char c = Serial1.read();
			c = DataReader.handleNextChar(c);
			if (-1 != c) {
				//Serial.write(c);
				DataParser.parseNextChar(c);
			}
		}
		time -= 1;
		delay(1);
	}
	
	Serial.println((*DataParser.getResultData()[0][0]));
	for (int i = 0; i < DataParser.getLengthOfDataResults()[1]; i++) {
		Serial.println((*DataParser.getResultData()[1][i]));
	}
	Serial.println("After parse: ");
	Serial.println(freeRam());
	DataParser.clearMemory();
	Serial.println("After clear: ");
	Serial.println(freeRam());
	Serial.print(DataReader.getLastHeader());
	
}


boolean esp_test() {
  return exec("AT\r\n").indexOf("OK") != -1;
}

String esp_get(String query) {
  String resp = exec("AT+CIPSTART=\"TCP\",\"" + SERVER_IP + "\"," +PORT+ "\r\n", 5000);
  if (resp.indexOf("OK") == -1) return "<error>" + resp + "</error>";
  /*String header = "GET " + query + " HTTP/1.1\r\nHOST: " + SERVER_IP + "\r\nAccept: application/json\r\n\r\n";*/
  String header = "GET " + query + " HTTP/1.1\r\n" + "HOST: " + SERVER_IP + "\r\n\r\n";
  resp = exec("AT+CIPSEND=" + String(header.length()) + "\r\n");
  if (resp.indexOf(">") == -1) return "<error2>" + resp + "</error2>";

  Serial.println("Before execBig: ");
  Serial.println(freeRam());
  exec_big_data(header);
  Serial.println(F("After execBig: "));
  Serial.println(freeRam());
  
}

enum main_states { FIRST_RUN, WAIT_A_MINUTE, WAIT_ESP_ANSWER, TEST_ESP_OK };

main_states main_state = FIRST_RUN;

byte light_state_cur = 0;

void blink_leds() {
  if (light_state_cur & RED_BLINK_MASK)
    digitalWrite(RED, !digitalRead(RED));
  if (light_state_cur & YELLOW_BLINK_MASK)
    digitalWrite(YELLOW, !digitalRead(YELLOW));
  if (light_state_cur & GREEN_BLINK_MASK)
    digitalWrite(GREEN, !digitalRead(GREEN));
}

void switch_state(byte state) {
  Timer1.stop();
  light_state_cur = state;
  pinWrite(RED, light_state_cur & RED_MASK);
  pinWrite(YELLOW, light_state_cur & YELLOW_MASK);
  pinWrite(GREEN, light_state_cur & GREEN_MASK);
  if (light_state_cur & BLINK_MASK)
    Timer1.restart();
}

void pinWrite(byte pin, boolean high) {
  if (high)
    digitalWrite(pin, HIGH);
  else
    digitalWrite(pin, LOW);
}

void serial1_find(char* str) {
  int len = strlen(str);
  int cur = 0;
  while (cur < len) {
    int c = Serial1.read();
    if (c != -1) {
      if (c == str[cur]) cur++;
    }
  }
}

String serial1_read_until(char term) {
  String res = "";
  while (1) {
    int c = Serial1.read();
    if (c != -1) {
      if (c == term) break;
      res += (char) c;
    }
  }
  return res;
}

String build_types[30];
int build_types_len;

void parse_build_types() {
  while (1) {
    int q = Serial1.read();
    if( q != -1) Serial.write(q);
  }
  
  build_types_len = 0;
  serial1_find("+IPD,");
  int len = serial1_read_until(':').toInt();
  
}

void wifi_setup(){
  exec("AT+CWJAP=\"" + WIFI_NAME + "\",\"" + WIFI_PASS + "\"\r\n",5000);
}


int freeRam()
{
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(ESP_RESET, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello");
  Serial1.begin(115200);
  Timer1.initialize(1000000);
  Timer1.stop();
  Timer1.attachInterrupt(blink_leds);
  esp_reset();
  wifi_setup();
}

void loop() {
  if (main_state == FIRST_RUN)
    switch_state(32);
  //esp_reset();
  switch_state(1);
  String wifi = exec("AT+CWJAP?\r\n");
  switch_state(4);
  if (wifi.indexOf(WIFI_NAME) == -1) {
    Serial.println(wifi);
    digitalWrite(YELLOW, HIGH);
    delay(1000);
    digitalWrite(YELLOW, LOW);
    return;
  }
  //Serial.print(esp_get("/guestAuth/app/rest/buildTypes/"));
  esp_get("/builds.jsp");
  //esp_get("/");
  switch_state(16);
 // parse_build_types();
}
