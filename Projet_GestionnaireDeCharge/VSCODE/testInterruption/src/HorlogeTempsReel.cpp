#include "HorlogeTempsReel.h"
#include <Arduino.h>
#include <time.h>

HorlogeTempsReel::HorlogeTempsReel()
{
  succes = false;
  alarmeDeclenchee = false;
  pinMode(PIN_ALARME, INPUT_PULLUP);
  anchor = this;

  if (RTC_DS3231::begin())
  {
    Serial.println("DS3231 initialisé.");
    succes = true;
    attachInterrupt(digitalPinToInterrupt(PIN_ALARME), HorlogeTempsReel::marshall, FALLING);
  }
  else
  {
    Serial.println("DS3231 introuvable !");
  }
}

void HorlogeTempsReel::syncFromNTP()
{
  bool ntpOk = false;
  struct tm timeinfo;
  int retries = 0;

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  while (!getLocalTime(&timeinfo) && retries < 20)
  {
    Serial.print(".");
    delay(500);
    retries++;
  }

  if (retries < 20)
  {
    ntpOk = true;
  }

  if (ntpOk)
  {
    adjust(DateTime(
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec));
    Serial.println("\nDS3231 synchronisé !");
  }
  else
  {
    Serial.println("\nÉchec NTP !");
  }
}

DateTime HorlogeTempsReel::getTime()
{
  return now();
}

void HorlogeTempsReel::printTime()
{
  DateTime maintenant = now();
  Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
                maintenant.day(), maintenant.month(), maintenant.year(),
                maintenant.hour(), maintenant.minute(), maintenant.second());
}

const bool HorlogeTempsReel::obtenirEtat()
{
  return succes;
}

void HorlogeTempsReel::configurerAlarmeMinute()
{
  disable32K();
  clearAlarm(1);
  clearAlarm(2);
  disableAlarm(2);

  // Ne pas mettre DS3231_OFF, la broche doit rester active
  writeSqwPinMode(DS3231_OFF); // Désactive le signal carré
  // Active l'interruption d'alarme via le registre de contrôle
  // En mettant DS3231_OFF + setAlarm1, le DS3231 active automatiquement INTCN

  if (!setAlarm1(now() + TimeSpan(60), DS3231_A1_Second))
  {
    Serial.println("Erreur configuration alarme !");
  }
  else
  {
    Serial.println("Alarme minute configurée !");
  }
}

void HorlogeTempsReel::reinitialiserAlarme()
{
  clearAlarm(1);
  // Replanifie l'alarme 60 secondes plus tard
  setAlarm1(now() + TimeSpan(60), DS3231_A1_Second);
}

bool HorlogeTempsReel::getAlarme()
{
  return alarmeDeclenchee;
}

void HorlogeTempsReel::setAlarme(bool alarme)
{
  alarmeDeclenchee = alarme;
}

void HorlogeTempsReel::marshall()
{
  anchor->onAlarme();
}

void IRAM_ATTR HorlogeTempsReel::onAlarme()
{
  alarmeDeclenchee = true;
  Serial.println("Interruption !");
}

HorlogeTempsReel *HorlogeTempsReel::anchor = NULL;