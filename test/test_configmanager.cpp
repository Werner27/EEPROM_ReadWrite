#include <AUnit.h>
#include <Arduino.h>
#include "ConfigManager.h"

test(write_and_read_byte) {
  byte testValue = 123;
  byte readValue = 0;
  ConfigManager::writeByte(testValue);
  ConfigManager::readByte(readValue);
  assertEqual(testValue, readValue);
}

test(write_and_read_float) {
  float testValue = 42.42;
  float readValue = 0.0;
  ConfigManager::writeFloat(testValue);
  ConfigManager::readFloat(readValue);
  assertEqual(testValue, readValue);
}

test(write_and_read_wifi) {
  WifiConf testConf = {"MySSID", "MyPass"};
  WifiConf readConf;
  ConfigManager::writeWifiConf(testConf);
  ConfigManager::readWifiConf(readConf);
  assertTrue(strcmp(testConf.ssid, readConf.ssid) == 0);
  assertTrue(strcmp(testConf.password, readConf.password) == 0);
}

test(write_and_read_mytestobject) {
  MyTestObject objWrite = {42, 987654, 3.14f, "Hello"};
  MyTestObject objRead;
  ConfigManager::writeMyTestObject(objWrite);
  ConfigManager::readMyTestObject(objRead);

  assertEqual(objWrite.byte_field, objRead.byte_field);
  assertEqual(objWrite.long_field, objRead.long_field);
  assertEqual(objWrite.float_field, objRead.float_field);
  assertTrue(strcmp(objWrite.name, objRead.name) == 0);
}

void setup() {
  Serial.begin(115200);
  ConfigManager::begin();
  aunit::TestRunner::run();
}

void loop() {
}
