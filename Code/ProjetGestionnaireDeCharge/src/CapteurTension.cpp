#include "CapteurTension.h"
#include <Arduino.h>

CapteurTension::CapteurTension(int pin) : brocheAnalogique(pin) {
    capteur = new ZMPT101B(pin);
}

void CapteurTension::initialiser() {
    Serial.println("--- Initialisation du Capteur ZMPT101B ---");
    capteur->setVref(3.3);
    int zero = capteur->calibrate();
    Serial.print("Point zero detecte : ");
    Serial.println(zero);
    capteur->setSensitivity(0.0129);
}

float CapteurTension::lireValeurTension() {
    float V = capteur->getVoltageAC();
    return (V < 15.0) ? 0.0 : V;
}
