#include "CapteurCourant.h"

CapteurCourant::CapteurCourant(int pin, float sens)
    : brocheAnalogique(pin), sensibilite(sens) {}

void CapteurCourant::initialiser() {
    emon.current(brocheAnalogique, sensibilite);
}

float CapteurCourant::lireValeurCourant() {
    return emon.calcIrms(1480);
}
