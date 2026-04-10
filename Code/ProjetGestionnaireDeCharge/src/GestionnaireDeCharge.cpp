#include "GestionnaireDeCharge.h"
#include <Arduino.h>

GestionnaireDeCharge::GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c, ConnectionRaspberryPi* r)
    : sensorTension(t), sensorCourant(c), raspi(r),sommeTension(0), sommeCourant(0),
      energieCumuleeWh(0), nombreLectures(0), chronoMinute(0) {}

void GestionnaireDeCharge::initialiser() {
    sensorTension->initialiser();
    sensorCourant->initialiser();
    chronoMinute = millis();
}



void GestionnaireDeCharge::envoyerMesures() {

    if (millis() - derniereMesure < 1000) return;
    derniereMesure = millis();

    float V = sensorTension->lireValeurTension();
    float I = sensorCourant->lireValeurCourant();
    float P = V * I;

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
    
        sommeTension = sommeCourant = 0;
        nombreLectures = 0;
        chronoMinute = millis();
    }

    
}

void GestionnaireDeCharge::verifierSeuils(float courant, float tension) {
    // Seuil de 18A (protection 20A ES-01)
    if (courant > 18.0) {
        Serial.printf("!!! ALERTE COURANT : %.2f A !!!\n", courant);
        
        // On utilise la nouvelle méthode de raspi (type 0 = courant)
        raspi->EnvoyerAlerte(0, "Surcharge detectee : " + String(courant) + "A");
    }
}