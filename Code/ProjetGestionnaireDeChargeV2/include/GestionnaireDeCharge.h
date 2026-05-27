#ifndef GESTIONNAIRE_DE_CHARGE_H
#define GESTIONNAIRE_DE_CHARGE_H

#include "ConnectionRaspberryPi.h"
#include "CapteurCourantTension.h"

class GestionnaireDeCharge {
private:
    ConnectionRaspberryPi*  raspi;
    CapteurCourantTension   capteur;

    float          energieCumuleeWh = 0;
    unsigned long  chronoMinute     = 0;
    unsigned long  derniereMesure   = 0;

public:
    GestionnaireDeCharge(ConnectionRaspberryPi* r);

    void effectuerMesures();
    void verifierSeuils(float courant);
};

#endif