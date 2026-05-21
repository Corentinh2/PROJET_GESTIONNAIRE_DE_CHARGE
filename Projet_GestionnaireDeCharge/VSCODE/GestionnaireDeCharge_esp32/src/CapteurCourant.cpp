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
void CapteurCourant::initialiser()
{
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
float CapteurCourant::lireValeurCourant()
{
    // 1. Déclaration de la variable locale en début de méthode
    float courantIrms = 0.0;

    // 2. Calcul via la bibliothèque EmonLib
    courantIrms = emon.calcIrms(1480);

    // 3. Filtrage du bruit de fond (Noise Gate)
    // On vérifie si la mesure est significative
    /*if (courantIrms < 0.5) {
        // Si on est en dessous d'un certain seuil (ex: 1.0A), on force à zéro
        courantIrms = 0.0;
    }*/

    // 4. Un seul return à la toute fin
    return courantIrms;
}

bool CapteurCourant::surveillerCourant()
{
    float courant = lireValeurCourant();
    bool retour = false;
    // Seuil de 18A (protection 20A ES-01)
    if (courant > SEUIL_MAX)
    {
        Serial.printf("!!! ALERTE COURANT : %.2f A !!!\n", courant);
        retour = true;
    }
    return retour;
}
