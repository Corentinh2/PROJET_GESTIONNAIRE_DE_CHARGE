#include "GestionnaireDeCharge.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include "ConnectionRaspberryPi.h"
#include "eps32_snir.h"
#define RELAIS 26

GestionnaireDeCharge *gestionnaire;
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("--- DEMARRAGE DU SYSTEME ---");
    pinMode(BP1,INPUT);
    pinMode(BP2,INPUT);

    delay(1000);
    pinMode(RELAIS,OUTPUT); 

    gestionnaire = new GestionnaireDeCharge;
    
}

void loop() {
    if (digitalRead(BP1)==LOW){
        digitalWrite(RELAIS,HIGH);
    }

    if (digitalRead(BP2)==LOW){
        digitalWrite(RELAIS,LOW);
    }
  gestionnaire->controler();
  //delay(500);
  // Serial.println(analogRead(32));

   /*static unsigned long chronoTest = 0;
    if (millis() - chronoTest > 10000) {
        Serial.println("\n[TEST] Simulation d'une surcharge à 25A...");
        systeme.verifierSeuils(25.0, 230.0);
        chronoTest = millis();
    }*/
}