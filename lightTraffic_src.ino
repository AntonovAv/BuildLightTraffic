#include "ReadDataOfIdsState.h"
#include "ReadIdsState.h"
#include "ConnectToWiFiState.h"
#include "SystemUtils.h"
#include "BasicLightStrategy.h"
#include "SystemState.h"
#include "LightTrafficSystem.h"

#include "ResetModuleState.h"

#include "JSONDataParser.h"
#include "DataReader.h"
#include <TimerOne.h>
#include <eeprom.h>

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
#define START_ADDRESS_OF_BUILD_IDS 199

#define ESP_RESET 2


/*
String esp_get(String query) {
  String resp = exec("AT+CIPSTART=\"TCP\",\"" + SERVER_IP + "\"," +PORT+ "\r\n", 5000);
  if (resp.indexOf("OK") == -1) return "<error>" + resp + "</error>";
  /*String header = "GET " + query + " HTTP/1.1\r\nHOST: " + SERVER_IP + "\r\nAccept: application/json\r\n\r\n";*/
  /*String header = "GET " + query + " HTTP/1.1\r\n" + "HOST: " + SERVER_IP + "\r\n\r\n";
  resp = exec("AT+CIPSEND=" + String(header.length()) + "\r\n");
  if (resp.indexOf(">") == -1) return "<error2>" + resp + "</error2>";

  Serial.println("Before execBig: ");
  Serial.println(freeRam());
  //exec_big_data(header);
  Serial.println(F("After execBig: "));
  Serial.println(freeRam());
  
}*/

enum main_states { FIRST_RUN, WAIT_A_MINUTE, WAIT_ESP_ANSWER, TEST_ESP_OK };

main_states main_state = FIRST_RUN;

byte light_state_cur = 0;

void blink_leds() {
	Serial.println("kuku");
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
  while (!Serial) {}
  Serial.println("Hello");
  Serial1.begin(115200);
  Timer1.initialize(1000); // 1 sec
  Timer1.stop();
  Timer1.attachInterrupt(blink_leds);

  Serial.print("currentFreeMemory(before start): "); Serial.println(freeRam());
}


LightTrafficSystem system = LightTrafficSystem(new ResetModuleState());

void loop() {
	Serial.print("currentFreeMemory: "); Serial.println(freeRam());
	//ResetModuleState rm;
	//rm.process();
	//rm.getNextState()->process();
	system.process();
	delay(5000);

 /* if (main_state == FIRST_RUN)
    switch_state(32);
  //esp_reset();
  switch_state(1);
  String wifi = exec(F("AT+CWJAP?\r\n"));
  switch_state(4);
  if (wifi.indexOf(WIFI_NAME) == -1) {
    Serial.println(wifi);
    digitalWrite(YELLOW, HIGH);
    delay(1000);
    digitalWrite(YELLOW, LOW);
    return;
  }
  //Serial.print(esp_get("/guestAuth/app/rest/buildTypes/"));
  execGetIdsOfConfigurations();
  //esp_get("/");
  switch_state(16);
  // parse_build_types();*/


}
