/**
 @file CapteurCourantTension.h
 @brief Déclaration de la classe CapteurCourantTension
 @version 1.0
 @author Valentin RUELLE
 @date 28/05/2026
 */

#ifndef CAPTEUR_COURANT_TENSION_H
#define CAPTEUR_COURANT_TENSION_H

#include "EmonLib.h"

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