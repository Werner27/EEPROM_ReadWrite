#include "CliManager.h"
#include "ConfigManager.h"

void CliManager::begin() {
#ifdef SERIAL_VERBOSE
    Serial.begin(115200);
    delay(100); // Warte auf serielle Verbindung
    DBG_PRINTLN();
    DBG_PRINTLN("CLI gestartet (Debug-Modus)");
    showMenu();
#endif
}

void CliManager::handle() {
#ifdef SERIAL_VERBOSE
    if (Serial.available()) {
        char cmd = Serial.read();
        processCommand(cmd);
    }
#endif
}

void CliManager::showMenu() {
#ifdef SERIAL_VERBOSE
    DBG_PRINTLN(F("\n=== CLI Menü ==="));
    DBG_PRINTLN(F("1 - WiFi-Daten lesen"));
    DBG_PRINTLN(F("2 - WiFi-Daten schreiben (Demo-Werte)"));
    DBG_PRINTLN(F("3 - Byte lesen"));
    DBG_PRINTLN(F("4 - Byte schreiben (42)"));
    DBG_PRINTLN(F("5 - Float lesen"));
    DBG_PRINTLN(F("6 - Float schreiben (3.14)"));
    DBG_PRINTLN(F("7 - MyTestObject lesen"));
    DBG_PRINTLN(F("8 - MyTestObject schreiben (Demo-Werte)"));
    DBG_PRINTLN(F("9 - Text lesen   (max 50 Zeichen)"));
    DBG_PRINTLN(F("a - Text schreiben (Abc123)   (max 50 Zeichen)"));
    DBG_PRINTLN(F("b - WiFi-Daten AP lesen"));
    DBG_PRINTLN(F("c - WiFi-Daten AP schreiben (Demo-Werte)"));
    DBG_PRINTLN(F("d - EEPROM-Dump"));
    DBG_PRINTLN(F("================"));
#endif
}

void CliManager::processCommand(char cmd) {
#ifdef SERIAL_VERBOSE
    switch (cmd) {
        case '1': {
            WifiConf conf;
            ConfigManager::readWifiConf(conf);
            break;
        }
        case '2': {
            WifiConf conf = { "MySSID", "MyPass" };
            ConfigManager::writeWifiConf(conf);
            break;
        }
        case '3': {
            byte b;
            ConfigManager::readByte(b);
            break;
        }
        case '4': {
            byte b = 42;
            ConfigManager::writeByte(b);
            break;
        }
        case '5': {
            float f;
            ConfigManager::readFloat(f);
            break;
        }
        case '6': {
            float f = 3.14f;
            ConfigManager::writeFloat(f);
            break;
        }
        case '7': {
            MyTestObject obj;
            ConfigManager::readMyTestObject(obj);
            break;
        }
        case '8': {
            MyTestObject obj = { 7, 123456, 1.23f, "TestName" };
            ConfigManager::writeMyTestObject(obj);
            break;
        }
        case '9': {
            String str;
            ConfigManager::readString(str);
            break;
        }
        case 'a': {
            String str = "Abc123";
            ConfigManager::writeString(str);
            break;
        }
        case 'b': {
            WifiConf conf;
            ConfigManager::readWifiConf(conf, true); // Access Point (AP)
            break;
        }
        case 'c': {
//            WifiConf conf = { "AP_SSID", "AP_Pass", IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0) };
            WifiConf conf = {}; // IP-Adressen werden auf Default gesetzt
            ConfigManager::writeWifiConf(conf, true); // Access Point (AP)
            break;
        }
        case 'd': { // 🔹 EEPROM-Dump mit ASCII
            DBG_PRINTLN(F("=== EEPROM Dump (HEX | ASCII) ==="));
            for (size_t i = 0; i < EEPROM_SIZE; i += 16) {
                // Offset
                if (i < 0x10) DBG_PRINT("0x0");
                else DBG_PRINT("0x");
                DBG_PRINT(i, HEX);
                DBG_PRINT(": ");

                // HEX-Teil
                for (size_t j = 0; j < 16; j++) {
                    if (i + j < EEPROM_SIZE) {
                        byte val = EEPROM.read(i + j);
                        if (val < 0x10) DBG_PRINT("0");
                        DBG_PRINT(val, HEX);
                        DBG_PRINT(" ");
                    } else {
                        DBG_PRINT("   "); // Padding
                    }
                }

                DBG_PRINT(" | ");

                // ASCII-Teil
                for (size_t j = 0; j < 16; j++) {
                    if (i + j < EEPROM_SIZE) {
                        byte val = EEPROM.read(i + j);
                        if (val >= 32 && val <= 126) {
                            // druckbares Zeichen
                            Serial.write(val);
                        } else {
                            Serial.write('.');
                        }
                    }
                }
                DBG_PRINTLN("");
            }
            DBG_PRINTLN(F("=== Ende Dump ==="));
            break;
}
        default:
            DBG_PRINTLN("Unbekanntes Kommando");
    }
    showMenu();
#endif
}
