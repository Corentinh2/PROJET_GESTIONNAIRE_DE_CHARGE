#ifndef HORLOGE_TEMPS_REEL_H
#define HORLOGE_TEMPS_REEL_H

#include <Wire.h>
#include <RTClib.h>

#define PIN_ALARME 23


class HorlogeTempsReel : public RTC_DS3231
{
private:
    bool succes;
    void IRAM_ATTR onAlarme();
    static void marshall();
    static HorlogeTempsReel* anchor;
    volatile bool alarmeDeclenchee;

public:

  HorlogeTempsReel();
  void syncFromNTP();
  DateTime getTime();
  void printTime();
  const bool obtenirEtat();
  void configurerAlarmeMinute();
  void reinitialiserAlarme();
  bool getAlarme();
  void setAlarme(bool _alarme);
  int obtenirJourSemaine();
};

#endif