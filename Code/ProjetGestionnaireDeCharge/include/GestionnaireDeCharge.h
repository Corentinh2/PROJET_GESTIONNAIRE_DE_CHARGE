#ifndef GESTIONNAIRE_DE_CHARGE_H
#define GESTIONNAIRE_DE_CHARGE_H

#include "CapteurTension.h"
#include "CapteurCourant.h"
#include "ConnectionRaspberryPi.h"

class GestionnaireDeCharge {
private:
    CapteurTension* sensorTension;
    CapteurCourant* sensorCourant;
    ConnectionRaspberryPi* raspi;

    float sommeTension = 0;
    float sommeCourant = 0;
    float energieCumuleeWh = 0;
    int nombreLectures = 0;
    unsigned long chronoMinute = 0;

public:
    GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c, ConnectionRaspberryPi* r);
    void initialiser();
    void envoyerMesures();
};

#endif
