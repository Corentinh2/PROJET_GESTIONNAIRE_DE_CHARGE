#ifndef CAPTEUR_TENSION_H
#define CAPTEUR_TENSION_H

#include <EmonLib.h>
#define CALIB 300.78
#define DEPHASAGE 1.7

class CapteurTension {
private:
    int brocheAnalogique;
    float calibration;
    float dephasage;

public:
    EnergyMonitor emon;
    CapteurTension(int pin,float calib, float phase);
    void initialiser();
    float lireValeurTension();
};

#endif
