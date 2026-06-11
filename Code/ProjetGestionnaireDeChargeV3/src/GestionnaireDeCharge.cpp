/**
 * @file GestionnaireDeCharge.cpp
 * @brief Implémentation de la classe GestionnaireDeCharge pour la supervision
 *        et l'envoi des mesures électriques.
 *
 * Ce fichier orchestre l'acquisition des données via CapteurCourantTension,
 * la détection de seuils critiques, et l'envoi de rapports vers le Raspberry Pi
 * via WebSocket.
 */

#include "GestionnaireDeCharge.h"
#include <Arduino.h>

/**
 * @brief Constructeur de la classe GestionnaireDeCharge.
 *
 * Initialise le pointeur vers la connexion Raspberry Pi et délègue
 * l'initialisation des capteurs à CapteurCourantTension.
 *
 * @param r  Pointeur vers l'objet ConnectionRaspberryPi pour l'envoi des données.
 */
GestionnaireDeCharge::GestionnaireDeCharge()
    :  energieCumuleeWh(0), chronoMinute(0), derniereMesure(0)
{

    raspi = new ConnectionRaspberryPi;
    capteur = new CapteurCourantTension;
    
    raspi->initialiserConnexion();
    chronoMinute = millis();
}

GestionnaireDeCharge::~GestionnaireDeCharge()
{
    delete raspi;
    delete capteur;
}

void GestionnaireDeCharge::controler()
{
    if (capteur->verifierSeuils() == true)
    {
        raspi->EnvoyerAlerte(0, "SurIntensite detecte");
    }
    capteur->effectuerMesures();
    raspi->EnvoyerMesures(capteur->getMoyennePuissance());

}

