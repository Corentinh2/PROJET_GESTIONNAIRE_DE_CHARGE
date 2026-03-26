#ifndef GESTIONNAIRE_DE_CHARGE_H
#define GESTIONNAIRE_DE_CHARGE_H

#include "CapteurTension.h"
#include "CapteurCourant.h"

class GestionnaireDeCharge {
private:
    CapteurTension* sensorTension;
    CapteurCourant* sensorCourant;
    float sommeTension;
    float sommeCourant;
    float energieCumuleeWh;
    int nombreLectures;
    unsigned long chronoMinute;

public:
    GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c);
    void initialiser();
    void envoyerMesures();
};

#endif
