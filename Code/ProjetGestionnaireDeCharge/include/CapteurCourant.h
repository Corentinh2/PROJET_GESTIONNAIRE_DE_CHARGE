#ifndef CAPTEUR_COURANT_H
#define CAPTEUR_COURANT_H

#include <EmonLib.h>

class CapteurCourant {
private:
    int brocheAnalogique;
    float sensibilite;
    EnergyMonitor emon;

public:
    CapteurCourant(int pin, float sens);
    void initialiser();
    float lireValeurCourant();
};

#endif
