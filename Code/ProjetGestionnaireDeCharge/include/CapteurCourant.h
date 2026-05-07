#ifndef CAPTEUR_COURANT_H
#define CAPTEUR_COURANT_H

#include <EmonLib.h>

class CapteurCourant {
private:
    int brocheAnalogique;
    float sensibilite;
    EnergyMonitor emon;
    const float SEUIL_MAX = 18.0;

public:
CapteurCourant() {} 
    CapteurCourant(int pin, float sens);
    void initialiser();
    float lireValeurCourant();
    void begin();
};

#endif
