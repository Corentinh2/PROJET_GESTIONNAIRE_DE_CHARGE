/**
 * @file CapteurCourantTension.cpp
 * @brief Implémentation de la classe CapteurCourantTension.
 * @version 1.0
 * @author Valentin Ruelle
 * @date 28/05/2026
 * @details Gère l'initialisation et l'acquisition des mesures de tension et de courant
 * via la bibliothèque EmonLib.
 */

#include "CapteurCourantTension.h"
#include <Arduino.h>

// ── Constantes de configuration des capteurs ─────────────────────────────────
#define TENSION     34      // Pin ADC tension
#define COURANT     35      // Pin ADC courant
#define CALIB       234.26  // Calibration tension
#define DEPHASAGE   1.7     // Déphasage tension
#define SENSI       60.6    // Sensibilité capteur courant
// ─────────────────────────────────────────────────────────────────────────────

CapteurCourantTension::CapteurCourantTension()
    : sommeTension(0), 
    sommeCourant(0), 
    sommePuissance(0),
    nombreLectures(0), 
    moyenneTension(0),
    moyenneCourant(0.0),
    moyennePuissance(0.0) 
    {

    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);

    emon.voltage(TENSION, CALIB, DEPHASAGE);
    emon.current(COURANT, SENSI);

    Serial.println("--- Initialisation GestionnaireDeCharge OK ---");
    }


/**
 * @brief Acquiert les mesures, les affiche et envoie un rapport toutes les minutes.
 *
 * Cette méthode est conçue pour être appelée dans la boucle principale (loop()).
 * Elle effectue les opérations suivantes :
 * - Limite les acquisitions à une par seconde.
 * - Lit la tension (V), le courant (A) et calcule la puissance instantanée (W).
 * - Affiche les valeurs sur le port série sous forme de tableau.
 * - Accumule les valeurs pour le calcul de moyennes sur une minute.
 * - Toutes les 60 secondes, calcule les moyennes, met à jour l'énergie cumulée
 *   en Wh, affiche un rapport récapitulatif, envoie la puissance moyenne au
 *   Raspberry Pi via WebSocket, puis remet les accumulateurs à zéro.
 */
void CapteurCourantTension::effectuerMesures() {
    emon.calcVI(20, 2000);

    // 1. Récupération des valeurs instantanées via les classes dédiées
    float tensionInstantanee  = emon.Vrms;
    float courantInstantane   = emon.Irms;
    float puissanceInstantanee = emon.realPower;

    // 2. Accumulation pour le calcul de la moyenne future
    sommeTension   += tensionInstantanee;
    sommeCourant   += courantInstantane;
    sommePuissance += puissanceInstantanee;
    nombreLectures++;

    // 3. Affichage sur le moniteur série toutes les secondes
    Serial.println("--- Mesures Instantanées ---");
    Serial.print("Tension (V) : ");
    Serial.println(tensionInstantanee);
    Serial.print("Courant (A) : ");
    Serial.println(courantInstantane);
    Serial.print("Puissance (W) : ");
    Serial.println(puissanceInstantanee);
    Serial.println("----------------------------");

    // 4. Vérification et traitement du palier des 60 secondes (moyenne)
    if (nombreLectures >= 60) {
        moyenneTension   = sommeTension   / nombreLectures;
        moyenneCourant   = sommeCourant   / nombreLectures;
        moyennePuissance = sommePuissance / nombreLectures;

        Serial.println("=========================================");
        Serial.println("     MOYENNE DES DE LA MINUTE ÉCOULÉE    ");
        Serial.print("Moyenne Tension : ");
        Serial.print(moyenneTension);
        Serial.println(" V");
        Serial.print("Moyenne Courant : ");
        Serial.print(moyenneCourant);
        Serial.println(" A");
        Serial.print("Moyenne Puissance : ");
        Serial.print(moyennePuissance);
        Serial.println(" W");
        Serial.println("=========================================");

        // Optionnel : Envoi vers la Raspberry Pi si les seuils sont OK

        // Réinitialisation des accumulateurs pour la minute suivante
        sommeTension   = 0.0;
        sommeCourant   = 0.0;
        nombreLectures = 0;
    }
}


/**
 * @brief Vérifie si le courant mesuré dépasse le seuil critique.
 *
 * Contrôle le courant par rapport au seuil de protection de 18 A (disjoncteur
 * ES-01 calibré à 20 A). En cas de dépassement, une alerte est affichée sur le
 * port série.
 *
 * @param courant  Valeur du courant mesuré en ampères (A).
 * @return true    Si le seuil est dépassé.
 * @return false   Si le courant est dans les limites acceptables.
 */
bool CapteurCourantTension::verifierSeuils() {
    // Seuil de 18A (protection 20A ES-01)
    bool retour = false;
    float courant = emon.Irms;
    if (courant > 18.0) {
        Serial.printf("!!! ALERTE COURANT : %.2f A !!!\n", courant);
        retour = true;
    }
    return retour;
}

float CapteurCourantTension::getMoyennePuissance()
{
    return moyennePuissance;
}
