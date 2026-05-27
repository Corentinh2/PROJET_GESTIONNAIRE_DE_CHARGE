#ifndef CAPTEUR_COURANT_TENSION_H
#define CAPTEUR_COURANT_TENSION_H

#include "EmonLib.h"

class CapteurCourantTension {
private:
    EnergyMonitor emon;

    float sommeTension = 0;
    float sommeCourant = 0;
    double sommePuissance = 0;
    int nombreLectures = 0;

public:
    CapteurCourantTension();

    void initialiserCapteursCourantTension();
    void effectuerMesures();

    // Accesseurs pour que GestionnaireDeCharge puisse lire les valeurs si besoin
    float getCourantInstantane() const;
    float getTensionInstantane() const;
    float getPuissanceInstantane() const;
};

#endif