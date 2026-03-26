#include "GestionnaireDeCharge.h"
#include <Arduino.h>

GestionnaireDeCharge::GestionnaireDeCharge(CapteurTension* t, CapteurCourant* c)
    : sensorTension(t), sensorCourant(c), sommeTension(0), sommeCourant(0),
      energieCumuleeWh(0), nombreLectures(0), chronoMinute(0) {}

void GestionnaireDeCharge::initialiser() {
    sensorTension->initialiser();
    sensorCourant->initialiser();
    chronoMinute = millis();
}

void GestionnaireDeCharge::envoyerMesures() {
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

        sommeTension = sommeCourant = 0;
        nombreLectures = 0;
        chronoMinute = millis();
    }

    delay(1000);
}
