/**
 * @file CapteurTension.cpp
 * @brief Implémentation de la classe CapteurTension pour la mesure de tension alternative.
 *
 * Ce fichier contient l'implémentation des méthodes de la classe CapteurTension,
 * basée sur le capteur ZMPT101B pour la mesure de tension secteur AC via une
 * broche analogique.
 */

#include "CapteurTension.h"
#include <Arduino.h>

/**
 * @brief Constructeur de la classe CapteurTension.
 *
 * Initialise le capteur de tension avec la broche analogique spécifiée et
 * instancie dynamiquement l'objet ZMPT101B associé.
 *
 * @param pin  Numéro de la broche analogique sur laquelle est branché le capteur ZMPT101B.
 */
CapteurTension::CapteurTension(int pin) : brocheAnalogique(pin) {
    capteur = new ZMPT101B(pin);
}

/**
 * @brief Initialise et calibre le capteur de tension ZMPT101B.
 *
 * Configure la tension de référence, effectue une calibration automatique pour
 * détecter le point zéro du signal AC, puis définit la sensibilité du capteur.
 * Les informations de calibration sont affichées sur le port série.
 *
 * @note Cette méthode doit être appelée une fois au démarrage, avant toute
 *       lecture de tension. Le port série doit être initialisé au préalable.
 */
void CapteurTension::initialiser() {
    Serial.println("--- Initialisation du Capteur ZMPT101B ---");
    capteur->setVref(3.3);
    int zero = capteur->calibrate();
    Serial.print("Point zero detecte : ");
    Serial.println(zero);
    capteur->setSensitivity(0.0157232);
}

/**
 * @brief Lit et retourne la valeur efficace (RMS) de la tension mesurée.
 *
 * Acquiert la tension AC via le capteur ZMPT101B. Les valeurs inférieures à
 * 15 V sont considérées comme du bruit et ramenées à 0 V afin d'éviter des
 * lectures parasites lorsqu'aucune tension secteur n'est présente.
 *
 * @return Valeur efficace de la tension en volts (V), ou 0.0 si la tension
 *         mesurée est inférieure à 15 V.
 */
float CapteurTension::lireValeurTension() {
    
    float vAc = capteur->getVoltageAC();

    // On retourne directement la valeur lue par le capteur 
    // sans filtrage de seuil
    return vAc;
    
    
    
    /*float tensionFinale = 0.0;
    float vAc = capteur->getVoltageAC();

    // On n'utilise qu'un seul IF simple
    // Si la tension est significative, on l'assigne
    if (vAc >= 15.0) {
        tensionFinale = vAc;
    }

    // Un seul point de sortie à la fin de la fonction
    return tensionFinale;
    */
}