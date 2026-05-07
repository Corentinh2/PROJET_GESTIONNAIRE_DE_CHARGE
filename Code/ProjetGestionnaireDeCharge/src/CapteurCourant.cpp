/**
 * @file CapteurCourant.cpp
 * @brief Implémentation de la classe CapteurCourant pour la mesure de courant alternatif.
 *
 * Ce fichier contient l'implémentation des méthodes de la classe CapteurCourant,
 * basée sur la bibliothèque EmonLib pour la mesure de courant RMS via un capteur
 * à effet Hall ou un transformateur de courant.
 */

#include "CapteurCourant.h"

/**
 * @brief Constructeur de la classe CapteurCourant.
 *
 * Initialise le capteur de courant avec la broche analogique et la sensibilité
 * du capteur utilisé.
 *
 * @param pin   Numéro de la broche analogique sur laquelle est branché le capteur.
 * @param sens  Sensibilité du capteur (ex. : ratio tours du transformateur de courant),
 *              utilisée pour convertir la tension lue en ampères.
 */
CapteurCourant::CapteurCourant(int pin, float sens)
    : brocheAnalogique(pin), sensibilite(sens) {}

/**
 * @brief Initialise le capteur de courant.
 *
 * Configure l'objet EmonLib avec la broche analogique et la sensibilité
 * définies à la construction. Doit être appelée avant toute lecture de courant.
 */
void CapteurCourant::initialiser() {
    emon.current(brocheAnalogique, sensibilite);
}

/**
 * @brief Lit et retourne la valeur efficace (RMS) du courant mesuré.
 *
 * Effectue le calcul du courant RMS à partir de 1480 échantillons acquis
 * sur la broche analogique configurée.
 *
 * @return Valeur efficace du courant en ampères (A).
 */

float CapteurCourant::lireValeurCourant() {
    
   
    emon.calcVI(20,2000);         // Calculate all. No.of half wavelengths (crossings), time-out
            // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)

  float Irms            = emon.Irms;             //extract Irms into Variable

  return Irms;
}

void CapteurCourant::begin() {
    

  emon.voltage(32, 234.26, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon.current(35, 20.35);
}
