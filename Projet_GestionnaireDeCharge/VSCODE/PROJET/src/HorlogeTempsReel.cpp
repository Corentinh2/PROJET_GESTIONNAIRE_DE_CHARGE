#include "HorlogeTempsReel.h"
#include <Arduino.h>
#include <time.h>

bool HorlogeTempsReel::begin() {
  bool succes = false;

  if (_rtc.begin()) {
    Serial.println("DS3231 initialisé.");
    succes = true;
  } else {
    Serial.println("DS3231 introuvable !");
  }

  return succes;
}

void HorlogeTempsReel::syncFromNTP() {
  bool ntpOk = false;
  struct tm timeinfo;
  int retries = 0;

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  while (!getLocalTime(&timeinfo) && retries < 20) {
    Serial.print(".");
    delay(500);
    retries++;
  }

  if (retries < 20) {
    ntpOk = true;
  }

  if (ntpOk) {
    _rtc.adjust(DateTime(
      timeinfo.tm_year + 1900,
      timeinfo.tm_mon + 1,
      timeinfo.tm_mday,
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    ));
    Serial.println("\nDS3231 synchronisé !");
  } else {
    Serial.println("\nÉchec NTP !");
  }
}

DateTime HorlogeTempsReel::getTime() {
  return _rtc.now();
}

void HorlogeTempsReel::printTime() {
  DateTime now = _rtc.now();
  Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
    now.day(), now.month(), now.year(),
    now.hour(), now.minute(), now.second()
  );
}