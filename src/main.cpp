#include <Arduino.h>
#include "ConfigManager.h"
#include "CliManager.h" 

#ifndef LED_BUILTIN 
  #define LED_BUILTIN 2 // Pin 2 für ESP8266 und  ESP32
#endif

IPAddress testIP(192,168,1,1);

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
int ledState = 0;

void setup() {
  CliManager::begin();
  ConfigManager::begin();
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  DBG_PRINT("LED_BUILTIN Pin Nr: "); DBG_PRINTLN(LED_BUILTIN);
}

void loop() {
    CliManager::handle();

  //loop to blink without delay
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    ledState = not(ledState);

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
}
