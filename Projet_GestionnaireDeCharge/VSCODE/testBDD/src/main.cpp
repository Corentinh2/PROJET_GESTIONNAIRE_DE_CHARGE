#include <Arduino.h>
#include <WiFi.h>
#include "GestionnaireCharge.h"


const char *ssid = "WIFI_BTS_CIEL";
const char *password = "BTSciel72?";

GestionnaireCharge *gestionnaire;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Démarrage...");
  Wire.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecté !");
  delay(1000);

  gestionnaire = new GestionnaireCharge;
  gestionnaire->obtenirHorloge()->syncFromNTP();


  // Efface d'abord manuellement avant de configurer
  gestionnaire->obtenirHorloge()->clearAlarm(1);
  gestionnaire->obtenirHorloge()->clearAlarm(2);
  gestionnaire->obtenirHorloge()->configurerAlarmeMinute();

  gestionnaire->afficherMenuTestUnitaire();
}

void loop() {


  if (gestionnaire->obtenirHorloge()->getAlarme()) {
    gestionnaire->obtenirHorloge()->setAlarme(false);
    gestionnaire->obtenirHorloge()->reinitialiserAlarme();

    DateTime maintenant = gestionnaire->obtenirHorloge()->getTime();
    int jour   = gestionnaire->obtenirHorloge()->obtenirJourSemaine();
    int heure  = maintenant.hour();
    int minute = maintenant.minute();
    bool debutTrouve = gestionnaire->obtenirMemoire()->lireCalendrier(jour, heure, minute, true);

    gestionnaire->obtenirHorloge()->printTime();
    if (debutTrouve) {
      Serial.println("Début de créneau → action début !");
      // Ton action début ici
    }

    if (!debutTrouve) {
      if (gestionnaire->obtenirMemoire()->lireCalendrier(jour, heure, minute, false)) {
        Serial.println("Fin de créneau → action fin !");
        // Ton action fin ici 
      }
    }
  }
}