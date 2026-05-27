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
GestionnaireDeCharge::GestionnaireDeCharge(ConnectionRaspberryPi* r)
    : raspi(r), energieCumuleeWh(0), chronoMinute(0), derniereMesure(0)
{
    capteur.initialiserCapteursCourantTension();
    chronoMinute = millis();
}

/**
 * @brief Délègue l'acquisition des mesures à CapteurCourantTension.
 *
 * Cette méthode est conçue pour être appelée dans la boucle principale (loop()).
 */
void GestionnaireDeCharge::effectuerMesures() {
    capteur.effectuerMesures();
}

/**
 * @brief Vérifie si les valeurs mesurées dépassent les seuils critiques.
 *
 * Contrôle le courant par rapport au seuil de protection de 18 A (disjoncteur
 * ES-01 calibré à 20 A). En cas de dépassement, une alerte est affichée sur le
 * port série et envoyée au Raspberry Pi via la méthode EnvoyerAlerte().
 *
 * @param courant  Valeur du courant mesuré en ampères (A).
 */
void GestionnaireDeCharge::verifierSeuils(float courant) {
    // Seuil de 18A (protection 20A ES-01)
    if (courant > 18.0) {
        Serial.printf("!!! ALERTE COURANT : %.2f A !!!\n", courant);

        // On utilise la nouvelle méthode de raspi (type 0 = courant)
        raspi->EnvoyerAlerte(0, "Surcharge detectee : " + String(courant) + "A");
    }
}