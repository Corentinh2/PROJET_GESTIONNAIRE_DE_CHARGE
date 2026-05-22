#ifndef GESTIONNAIRE_DE_CHARGE_H
#define GESTIONNAIRE_DE_CHARGE_H

#include "CapteurTension.h"
#include "CapteurCourant.h"
#include "ConnectionRaspberryPi.h"
#include "EmonLib.h"

class GestionnaireDeCharge {
private:
    CapteurTension* sensorTension;
    CapteurCourant* sensorCourant;
    ConnectionRaspberryPi* raspi;

    EnergyMonitor emon;

    float sommeTension = 0;
    float sommeCourant = 0;
    double sommePuissance = 0;
    float energieCumuleeWh = 0;
    int nombreLectures = 0;
    unsigned long chronoMinute = 0;
    unsigned long derniereMesure = 0;

public:
    GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c, ConnectionRaspberryPi* r);
    void initialiserCapteursCourantTension();
    void verifierSeuils(float courant);
    void effectuerMesures();
};

#endif
