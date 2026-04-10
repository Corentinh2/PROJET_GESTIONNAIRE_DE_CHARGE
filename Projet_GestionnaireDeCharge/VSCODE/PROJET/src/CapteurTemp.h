#ifndef CAPTEUR_TEMP_H
#define CAPTEUR_TEMP_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define PIN_DS18S20 18

class CapteurTemp {
private:
    OneWire oneWire;
    DallasTemperature capteur;
    int8_t sMax;

public:
    CapteurTemp(int8_t seuilMax);
    bool surveillerTemperature();
};

#endif