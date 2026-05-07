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
      energieCumuleeWh(0), nombreLectures(0), chronoMinute(0) {}

/**
 * @brief Initialise les capteurs de tension et de courant.
 *
 * Appelle successivement les méthodes d'initialisation des deux capteurs et
 * démarre le chronomètre utilisé pour le rapport à la minute.
 *
 * @note Doit être appelée une fois dans le setup() avant la boucle principale.
 */
void GestionnaireDeCharge::initialiser() {
    sensorTension->initialiser();
    sensorCourant->initialiser();
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
void GestionnaireDeCharge::envoyerMesures() {

// On englobe tout dans un bloc if pour éviter le return prématuré
    if (millis() - derniereMesure >= 1000) {
        derniereMesure = millis();

        float V = sensorTension->lireValeurTension();
        float I = sensorCourant->lireValeurCourant();
        float P = V * I;

        // APPEL DE LA VÉRIFICATION (Juste après la lecture)
        verifierSeuils(I);

        Serial.println("Tension (V)\t| Courant (A)\t| Puissance (W)");
        Serial.println("----------------|---------------|---------------");
        Serial.print(V, 1); Serial.print(" V\t| ");
        Serial.print(I, 2); Serial.print(" A\t| ");
        Serial.print(P, 1); Serial.println(" W");
        Serial.println("------------------------------------------------");

        sommeTension += V;
        sommeCourant += I;
        nombreLectures++;

        if (millis() - chronoMinute >= 60000) {
            float moyV = sommeTension / nombreLectures;
            float moyI = sommeCourant / nombreLectures;
            float moyP = moyV * moyI;
            energieCumuleeWh += (moyP / 60.0);

            Serial.println("\n--- RAPPORT DE LA DERNIERE MINUTE ---");
            Serial.println("U Moy (V) | I Moy (A) | P Moy (W) | Conso (Wh)");
            Serial.print(moyV, 1); Serial.print("\t| ");
            Serial.print(moyI, 2); Serial.print("\t| ");
            Serial.print(moyP, 1); Serial.print("\t| ");
            Serial.print(energieCumuleeWh, 3); Serial.println(" Wh");

            String trame = "{\"puissance\":" + String(moyP, 2) + "}";
            Serial.println("[WS] Envoi trame : " + trame);
            raspi->EnvoyerNotification(trame);
        
            sommeTension = 0;
            sommeCourant = 0;
            nombreLectures = 0;
            chronoMinute = millis();
        }
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
    if (courant > 1) {
        Serial.printf("!!! ALERTE COURANT : %.2f A !!!\n", courant);
        
        // On utilise la nouvelle méthode de raspi (type 0 = courant)
        raspi->EnvoyerAlerte(0, "Surcharge detectee : " + String(courant) + "A");
    }
}