#ifndef CAPTEUR_COURANT_TENSION_H
#define CAPTEUR_COURANT_TENSION_H

#include "EmonLib.h"
#include "constantes.h"

class CapteurCourantTension {
private:
    EnergyMonitor emon;

    float sommeTension;
    float sommeCourant;
    double sommePuissance;
    int nombreLectures;

    float moyenneTension;
    float moyenneCourant;
    float moyennePuissance;

public:
    CapteurCourantTension();

    void effectuerMesures();
    bool verifierSeuils();

    float getMoyennePuissance();
};

#endif