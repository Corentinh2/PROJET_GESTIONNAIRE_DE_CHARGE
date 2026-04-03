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
}


/*
CapteurTension capteurTension(32);   // Exemple : broche 32
CapteurCourant capteurCourant(33, 12.0);
GestionnaireDeCharge systeme(&capteurTension, &capteurCourant);
const char* ssid     = "fortinet";
const char* password = "Projet2026";
void setup() {
  Serial.begin(115200);
  systeme.initialiser();
  // --- Connexion WiFi ---
  Serial.println("\n--- Tentative de connexion Wi-Fi ---");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  systeme.envoyerMesures();
}
*/