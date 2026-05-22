/**
 * @file GestionnaireDeCharge.cpp
 * @brief Implémentation de la classe GestionnaireDeCharge pour la supervision
 *        et l'envoi des mesures électriques.
 *
 * Ce fichier gère l'acquisition périodique des données de tension et de courant,
 * le calcul de la puissance et de l'énergie cumulée, l'envoi de rapports minute
 * vers le Raspberry Pi via WebSocket, ainsi que la détection de seuils critiques.
 */

#include "GestionnaireDeCharge.h"
#include <Arduino.h>

/**
 * @brief Constructeur de la classe GestionnaireDeCharge.
 *
 * Initialise les pointeurs vers les capteurs et la connexion Raspberry Pi,
 * et remet à zéro tous les accumulateurs et compteurs internes.
 *
 * @param t  Pointeur vers l'objet CapteurTension utilisé pour les mesures de tension.
 * @param c  Pointeur vers l'objet CapteurCourant utilisé pour les mesures de courant.
 * @param r  Pointeur vers l'objet ConnectionRaspberryPi pour l'envoi des données.
 */
GestionnaireDeCharge::GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c, ConnectionRaspberryPi* r)
    : sensorTension(t), sensorCourant(c), raspi(r),sommeTension(0), sommeCourant(0),
      energieCumuleeWh(0), nombreLectures(0), chronoMinute(0) {
        initialiserCapteursCourantTension();
      }

/**
 * @brief Initialise les capteurs de tension et de courant.
 *
 * Appelle successivement les méthodes d'initialisation des deux capteurs et
 * démarre le chronomètre utilisé pour le rapport à la minute.
 *
 * @note Doit être appelée une fois dans le setup() avant la boucle principale.
 */
void GestionnaireDeCharge::initialiserCapteursCourantTension() {
      analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
 
    emon.voltage(TENSION, CALIB, DEPHASAGE);
    emon.current(COURANT, SENSI);
 
    Serial.println("--- Initialisation GestionnaireDeCharge OK ---");
    chronoMinute = millis();

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

void GestionnaireDeCharge::effectuerMesures() {

    emon.calcVI(20,2000);
// 1. Récupération des valeurs instantanées via les classes dédiées
    float tensionInstantanee = emon.Vrms;
    float courantInstantane = emon.Irms;
    float puissanceInstantanee = emon.realPower;

    // 2. Accumulation pour le calcul de la moyenne future
    sommeTension += tensionInstantanee;
    sommeCourant += courantInstantane;
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
        float moyenneTension = sommeTension / nombreLectures;
        float moyenneCourant = sommeCourant / nombreLectures;
        float moyennePuissance = sommePuissance / nombreLectures;

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
        // raspi->EnvoyerMesures(moyenneTension, moyenneCourant, moyennePuissance);

        // Réinitialisation des accumulateurs pour la minute suivante
        sommeTension = 0.0;
        sommeCourant = 0.0;
        nombreLectures = 0;
    }
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