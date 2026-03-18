#include "GestionnaireCharge.h"
#include <Arduino.h>

bool GestionnaireCharge::begin() {
  bool succes = true;

  if (!horloge.begin()) {
    Serial.println("Erreur horloge !");
    succes = false;
  }

  if (succes && !memoire.begin()) {
    Serial.println("Erreur mémoire programme !");
    succes = false;
  }

  return succes;
}
