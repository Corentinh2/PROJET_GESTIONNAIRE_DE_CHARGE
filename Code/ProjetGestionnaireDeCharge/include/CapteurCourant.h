#ifndef CAPTEUR_COURANT_H
#define CAPTEUR_COURANT_H

#include <EmonLib.h>
#include "eps32_snir.h"
#define SENSI 16.59
#define SEUIL_MAX 18.0

class CapteurCourant {
private:
    int brocheAnalogique;
    float sensibilite;

public:
    EnergyMonitor emon;
    CapteurCourant(int pin, float sens);
    void initialiser();
    float lireValeurCourant();
};

#endif
