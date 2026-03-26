#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>

String formatMAC(const uint8_t* mac) {
  char buf[18];
  snprintf(buf, sizeof(buf),
           "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("========================================");
  Serial.println("   ESP32 Lolin32 - Adresse MAC");
  Serial.println("========================================");

  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin();

  uint8_t macSTA[6];
  esp_wifi_get_mac(WIFI_IF_STA, macSTA);
  Serial.print("MAC WiFi Station  : ");
  Serial.println(formatMAC(macSTA));

  uint8_t macAP[6];
  esp_wifi_get_mac(WIFI_IF_AP, macAP);
  Serial.print("MAC WiFi AP       : ");
  Serial.println(formatMAC(macAP));

  Serial.print("MAC WiFi (lib)    : ");
  Serial.println(WiFi.macAddress());

  uint8_t macBT[6];
  memcpy(macBT, macSTA, 6);
  macBT[5] += 2;
  Serial.print("MAC Bluetooth     : ");
  Serial.println(formatMAC(macBT));

  Serial.print("eFuse MAC brute   : 0x");
  Serial.println(ESP.getEfuseMac(), HEX);

  Serial.println("========================================");
}

void loop() {
  delay(10000);
}