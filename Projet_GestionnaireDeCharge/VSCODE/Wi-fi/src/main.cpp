#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <RTClib.h>  // Bibliothèque Adafruit RTClib

const char* ssid     = "fortinet";
const char* password = "Projet2026";

// Serveurs NTP et fuseau horaire (France = UTC+1, UTC+2 en été)
const char* ntpServer1   = "pool.ntp.org";
const char* ntpServer2   = "time.nist.gov";
const long  gmtOffset_sec     = 3600;  // UTC+1
const int   daylightOffset_sec = 0;    // Ne pas ajouter l'heure d'été manuellement

RTC_DS3231 rtc;

void syncRTCfromNTP() {
  Serial.println("Synchronisation NTP en cours...");

  // Configure et démarre la synchro NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Attend que l'heure NTP soit disponible (max ~10 secondes)
  struct tm timeinfo;
  int retries = 0;
  while (!getLocalTime(&timeinfo) && retries < 20) {
    Serial.print(".");
    delay(500);
    retries++;
  }

  if (retries >= 20) {
    Serial.println("\nÉchec de la synchronisation NTP !");
    return;
  }

  Serial.println("\nHeure NTP reçue :");
  Serial.println(&timeinfo, "%A %d %B %Y %H:%M:%S");

  // Applique l'heure au DS3231
  rtc.adjust(DateTime(
    timeinfo.tm_year + 1900,
    timeinfo.tm_mon + 1,
    timeinfo.tm_mday,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  ));

  Serial.println("DS3231 synchronisé avec succès !");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // --- Init I2C et RTC ---
  Wire.begin();  // SDA=21, SCL=22 par défaut sur Lolin32
  if (!rtc.begin()) {
    Serial.println("DS3231 introuvable ! Vérifie le câblage.");
  }

  // --- Connexion WiFi ---
  Serial.println("\n--- Tentative de connexion Wi-Fi ---");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // --- Synchro NTP → DS3231 ---
  syncRTCfromNTP();

  // Déconnexion WiFi optionnelle pour économiser l'énergie
  // WiFi.disconnect(true);
}

void loop() {
  // Lecture de l'heure depuis le DS3231 toutes les secondes
  DateTime now = rtc.now();

  Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second()
  );

  delay(1000);
}