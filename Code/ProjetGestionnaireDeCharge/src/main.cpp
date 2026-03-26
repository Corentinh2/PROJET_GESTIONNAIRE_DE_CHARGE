#include "GestionnaireDeCharge.h"
#include <WiFi.h>
#include <esp_wifi.h>

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
