#include "GestionnaireDeCharge.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include "CapteurTension.h"
#include "CapteurCourant.h"
#include "ConnectionRaspberryPi.h"
#define RELAIS 26



// 1. Création des objets
CapteurTension capteurTension(32, 300.78, 1.7);  // broche, calibration, déphasage
CapteurCourant capteurCourant(33, 16.59);
ConnectionRaspberryPi raspi; // Ton nouvel objet réseau
GestionnaireDeCharge systeme(&capteurTension, &capteurCourant, &raspi);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("--- DEMARRAGE DU SYSTEME ---");
    raspi.initialiserConnexion(); // Nom de ton diagramme
    pinMode(BP1,INPUT);
    pinMode(BP2,INPUT);

    delay(1000);
    pinMode(RELAIS,OUTPUT);
    

    
}

void loop() {
    if (digitalRead(BP1)==LOW){
        digitalWrite(RELAIS,HIGH);
    }

    if (digitalRead(BP2)==LOW){
        digitalWrite(RELAIS,LOW);
    }
  // 4. On appelle la méthode qui traite et envoie si besoin
  systeme.effectuerMesures(); // Calcul et envoi automatique
  raspi.maintenirConnexion();
  //delay(500);
  // Serial.println(analogRead(32));

   /*static unsigned long chronoTest = 0;
    if (millis() - chronoTest > 10000) {
        Serial.println("\n[TEST] Simulation d'une surcharge à 25A...");
        systeme.verifierSeuils(25.0, 230.0);
        chronoTest = millis();
    }*/
}