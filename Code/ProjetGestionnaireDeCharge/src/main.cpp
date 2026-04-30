#include "GestionnaireDeCharge.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include "CapteurTension.h"
#include "CapteurCourant.h"
#include "ConnectionRaspberryPi.h"



// 1. Création des objets
CapteurTension capteurTension(32); 
CapteurCourant capteurCourant(33, 12.0);
ConnectionRaspberryPi raspi; // Ton nouvel objet réseau
GestionnaireDeCharge systeme(&capteurTension, &capteurCourant, &raspi);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("--- DEMARRAGE DU SYSTEME ---");
    raspi.initialiserConnexion(); // Nom de ton diagramme
    systeme.initialiser();
}

void loop() {
  // 4. On appelle la méthode qui traite et envoie si besoin
  systeme.envoyerMesures(); // Calcul et envoi automatique
  raspi.loop();

   /*static unsigned long chronoTest = 0;
    if (millis() - chronoTest > 10000) {
        Serial.println("\n[TEST] Simulation d'une surcharge à 25A...");
        systeme.verifierSeuils(25.0, 230.0);
        chronoTest = millis();
    }*/
}