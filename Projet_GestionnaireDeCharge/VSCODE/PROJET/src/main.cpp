#include <Arduino.h>
#include <WiFi.h>
#include "GestionnaireCharge.h"

const char* ssid     = "WIFI_BTS_CIEL";
const char* password = "BTSciel72?";

GestionnaireCharge gestionnaire;
//HorlogeTempsReel horloge;
//MemoireProgramme memoire;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Démarrage");
  Wire.begin();

  // WiFi en premier pour que NTP soit dispo ensuite
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté !");

  delay(1000);  // Laisse le temps à la connexion de se stabiliser

  if (!gestionnaire.begin()) {
    Serial.println("Erreur gestionnaire !");
    while(1);
  }
  Serial.println("Gestionnaire OK !");  // Ajoute cette ligne
  gestionnaire.horloge.syncFromNTP();  
  //gestionnaire.memoire.ajouterCalendrier();
  gestionnaire.memoire.lireCalendrier();
}

void loop() {
}