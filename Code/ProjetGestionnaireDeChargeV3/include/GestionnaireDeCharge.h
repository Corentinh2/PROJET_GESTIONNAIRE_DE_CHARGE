/**
 @file GestionnaireDeCharge.h
 @brief Déclaration de la classe GestionnaireDeCharge
 @version 1.0
 @author Valentin RUELLE
 @date 28/05/2026
 */

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