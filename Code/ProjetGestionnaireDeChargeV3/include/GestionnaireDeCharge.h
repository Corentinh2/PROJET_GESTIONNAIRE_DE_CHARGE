#ifndef GESTIONNAIRE_DE_CHARGE_H
#define GESTIONNAIRE_DE_CHARGE_H

#include "ConnectionRaspberryPi.h"
#include "CapteurCourantTension.h"

class GestionnaireDeCharge {
private:
    ConnectionRaspberryPi  *raspi;
    CapteurCourantTension  *capteur;

    float          energieCumuleeWh;
    unsigned long  chronoMinute;
    unsigned long  derniereMesure;

public:
    GestionnaireDeCharge();
    ~GestionnaireDeCharge();


    void controler();
};

#endif