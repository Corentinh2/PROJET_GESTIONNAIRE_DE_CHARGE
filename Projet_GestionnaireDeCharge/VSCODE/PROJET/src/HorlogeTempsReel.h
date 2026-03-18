#ifndef HORLOGE_TEMPS_REEL_H
#define HORLOGE_TEMPS_REEL_H

#include <Wire.h>
#include <RTClib.h>

class HorlogeTempsReel {
  private:
    RTC_DS3231 _rtc;

  public:
    bool begin();
    void syncFromNTP();
    DateTime getTime();
    void printTime();
};

#endif