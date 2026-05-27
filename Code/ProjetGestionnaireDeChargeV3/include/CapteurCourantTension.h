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

    float moyenneTension = 0.0;
    float moyenneCourant = 0.0;
    float moyennePuissance = 0.0;

public:
    CapteurCourantTension();

    void initialiserCapteursCourantTension();
    void effectuerMesures();
    bool verifierSeuils();

    float getMoyenneTension();
    float getMoyenneCourant();
    float getMoyennePuissance();
};

#endif