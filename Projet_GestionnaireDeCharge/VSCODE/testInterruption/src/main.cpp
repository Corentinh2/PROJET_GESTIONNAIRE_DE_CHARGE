#include <Arduino.h>
#include <WiFi.h>
#include "GestionnaireCharge.h"



const char *ssid = "WIFI_BTS_CIEL";
const char *password = "BTSciel72?";

GestionnaireCharge *gestionnaire;


/*
volatile bool alarmeDeclenchee = false;


void IRAM_ATTR onAlarme()
{
  alarmeDeclenchee = true;
}
*/



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
}

void loop()
{
#if 0
  // Debug - affiche l'état de la broche toutes les 2 secondes
  Serial.print("SQW : ");
  Serial.print(digitalRead(PIN_ALARME));
  Serial.print(" | alarmFired : ");
  Serial.println(gestionnaire->obtenirHorloge()->alarmFired(1));
  delay(2000);
#endif

  
  if (gestionnaire->obtenirHorloge()->getAlarme())
  {
    gestionnaire->obtenirHorloge()->setAlarme(false);
    gestionnaire->obtenirHorloge()->reinitialiserAlarme();
    //Serial.println("Nouvelle minute !");
    //gestionnaire->obtenirHorloge()->printTime();
    gestionnaire->obtenirMemoire()->lireCalendrier();
  }


  //UTILISER GETTER ET SETTER POUR AVOIR ACCES AU FLAG alarmeDeclenchee de la classe horloge
    
}