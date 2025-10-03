#include "ConfigManager.h"

void ConfigManager::begin() {
  EEPROM.begin(EEPROM_SIZE);
  DBG_PRINT("EEPROM length: ");
  DBG_PRINTLN(EEPROM.length());
}

void ConfigManager::ip_byte_to_array(const IPAddress &ip, byte ip_bytes[4]) {
    for (int i = 0; i < 4; i++) {
        ip_bytes[i] = ip[i];
    }
}
IPAddress ConfigManager::ip_from_bytes(const byte ip_bytes[4]) {
    return IPAddress(ip_bytes[0], ip_bytes[1], ip_bytes[2], ip_bytes[3]);
} 

IPAddress ConfigManager::CheckIP(const IPAddress &ip, const IPAddress defaultIP) {
    // Überprüfen, ob die Adresse die Standard ist ODER ob sie die Broadcast-Adresse ist
    if (ip == defaultIP || ip == IPAddress(255, 255, 255, 255)) {
        DBG_PRINTLN("WARNUNG: Ungültige oder Standard IP-Adresse erkannt. Setze auf Standard-IP.");
        return defaultIP; // Rückgabe der Standard-IP
    } else {
      // Optional: Führen Sie hier eine zusätzliche Prüfung durch, 
      // z.B. ob es sich um eine gültige private Adresse handelt
      if (ip[0] != 192 || ip[1] != 168) {
          DBG_PRINTLN("WARNUNG: IP liegt außerhalb des erwarteten Netzwerks (192.168.x.x).");
          return defaultIP; // Rückgabe der Standard-IP
      }
    }
    return ip; // Rückgabe der gültigen IP
}
IPAddress ConfigManager::CheckSN(const IPAddress &sn) {
    // Definieren der Null-IP-Adresse
    const IPAddress defaultSN(255, 255, 255, 0);
    
    // Überprüfen, ob die Adresse die Null-IP ist ODER ob sie die Broadcast-Adresse ist
    if (sn[0] != 255 || sn[1] != 255 || sn[3] != 0) {
        DBG_PRINTLN("WARNUNG: Ungültige oder leere SN-Adresse erkannt. Setze auf Standard-SN.");
        return defaultSN; // Rückgabe der Standard-SN
    }
    return sn; // Rückgabe der gültigen SN
} 

void ConfigManager::readWifiConf(WifiConf &wifiConf, bool ap) {
  if (ap) {
    EEPROM.get(EE_ADD_WIFI_AP, wifiConf);
    wifiConf.ip = CheckIP(wifiConf.ip, IPAddress(192, 168, 10, 1)); // Default für AP
    wifiConf.gw = CheckIP(wifiConf.gw, IPAddress(192, 168, 10, 1)); // Default nur für AP
  } else {
    EEPROM.get(EE_ADD_WIFI, wifiConf);
    wifiConf.ip = CheckIP(wifiConf.ip, IPAddress(0, 0, 0, 0)); // Null-IP als Default
    wifiConf.gw = IPAddress(0, 0, 0, 0); // Nur für AP relevant wird auf 0 gesetzt
  }   
  // Sicherstellen, dass die Strings terminiert sind
  wifiConf.ssid[MAX_SSID-1] = '\0';
  wifiConf.password[MAX_PASSWORD-1] = '\0';
  wifiConf.sn = CheckSN(wifiConf.sn);

  DBG_PRINTF("'%s' gelesene WLAN-Daten: SSID='%s', PASS='%s', IP='%d.%d.%d.%d', GW='%d.%d.%d.%d',SN='%d.%d.%d.%d'\n", 
            ap ? "Access Point (AP)" : "Client (STA)",
            wifiConf.ssid, wifiConf.password, 
            wifiConf.ip[0], wifiConf.ip[1],wifiConf.ip[2],wifiConf.ip[3],
            wifiConf.gw[0], wifiConf.gw[1],wifiConf.gw[2],wifiConf.gw[3],
            wifiConf.sn[0], wifiConf.sn[1],wifiConf.sn[2],wifiConf.sn[3]);
}

void ConfigManager::writeWifiConf(const WifiConf &wifiConf, bool ap) {
  DBG_PRINTF("'%s' gespeicherte WLAN-Daten: SSID='%s', PASS='%s', IP='%d.%d.%d.%d', GW='%d.%d.%d.%d',SN='%d.%d.%d.%d'\n", 
            ap ? "Access Point (AP)" : "Client (STA)",
            wifiConf.ssid, wifiConf.password, 
            wifiConf.ip[0], wifiConf.ip[1],wifiConf.ip[2],wifiConf.ip[3],
            wifiConf.gw[0], wifiConf.gw[1],wifiConf.gw[2],wifiConf.gw[3],
            wifiConf.sn[0], wifiConf.sn[1],wifiConf.sn[2],wifiConf.sn[3]);
            
  EEPROM.put(ap ? EE_ADD_WIFI_AP : EE_ADD_WIFI, wifiConf);
  EEPROM.commit();
}

void ConfigManager::readByte(byte &b) {
  b = EEPROM.read(EE_ADD_BYTE);
  DBG_PRINT("Byte: "); DBG_PRINTLN(b);
}

void ConfigManager::writeByte(const byte &b) {
  DBG_PRINT("Byte: "); DBG_PRINTLN(b);
  EEPROM.write(EE_ADD_BYTE, b);
  EEPROM.commit();
}

void ConfigManager::readFloat(float &f) {
  EEPROM.get(EE_ADD_FLOAT, f);
  DBG_PRINT("Float: "); DBG_PRINTLN(f, 3);
}

void ConfigManager::writeFloat(const float &f) {
  DBG_PRINT("Float: "); DBG_PRINTLN(f, 3);
  EEPROM.put(EE_ADD_FLOAT, f);
  EEPROM.commit();
}

void ConfigManager::readString(String &text) {
  char textBuffer[MAX_TEXT];
  EEPROM.get(EE_ADD_TEXT, textBuffer);
  textBuffer[MAX_TEXT-1] = '\0';
  text = String(textBuffer);
  DBG_PRINT("String: "); DBG_PRINTLN(text);
}

void ConfigManager::writeString(const String &text) {
  DBG_PRINT("String: "); DBG_PRINTLN(text);
  char textBuffer[MAX_TEXT] = {0};
  strncpy(textBuffer, text.c_str(), MAX_TEXT-1);
  EEPROM.put(EE_ADD_TEXT, textBuffer);
  EEPROM.commit();
}

void ConfigManager::readMyTestObject(MyTestObject &obj) {
  EEPROM.get(EE_ADD_MY_TEST_OBJECT, obj);
  DBG_PRINT("byte_field: "); DBG_PRINTLN(obj.byte_field);
  DBG_PRINT("long_field: "); DBG_PRINTLN(obj.long_field);
  DBG_PRINT("float_field: "); DBG_PRINTLN(obj.float_field);
  DBG_PRINT("name: "); DBG_PRINTLN(obj.name);
}

void ConfigManager::writeMyTestObject(const MyTestObject &obj) {
  DBG_PRINT("byte_field: "); DBG_PRINTLN(obj.byte_field);
  DBG_PRINT("long_field: "); DBG_PRINTLN(obj.long_field);
  DBG_PRINT("float_field: "); DBG_PRINTLN(obj.float_field);
  DBG_PRINT("name: "); DBG_PRINTLN(obj.name);

  EEPROM.put(EE_ADD_MY_TEST_OBJECT, obj);
  EEPROM.commit();
}