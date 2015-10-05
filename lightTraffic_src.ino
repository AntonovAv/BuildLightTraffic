#include "WiFiConnectionErrorLightStrategy.h"
#include "BuildServerRequestErrorLightStrategy.h"
#include "BuildsFailedAndRunningLightStrategy.h"
#include "BuildsFailedLightStrategy.h"
#include "BuildServerErrorLightStrategy.h"
#include "InitSystemLightStrategy.h"
#include "BuildsSuccessLightStrategy.h"
#include "BuildServerErrorState.h"
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

LightTrafficSystem system = LightTrafficSystem(new ResetModuleState(), new InitSystemLightStrategy());

void light() {
	system.lighting();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(ESP_RESET, OUTPUT);
  Serial.begin(115200);
  //while (!Serial) {}
  Serial.println("Hello");
  Serial1.begin(115200);
  Timer1.initialize(1000000/COEFF_FOR_1SEC); // 1 sec/COEFF sec
  //Timer1.stop();
  Timer1.attachInterrupt(light);

  Serial.print(F("currentFreeMemory(before start): ")); Serial.println(SystemUtils.freeRam());
}

void loop() {

	Serial.print(F("currentFreeMemory: ")); Serial.println(SystemUtils.freeRam());
	system.process();
}
