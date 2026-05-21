#ifndef CAPTEUR_TENSION_H
#define CAPTEUR_TENSION_H

#include <ZMPT101B.h>

class CapteurTension {
private:
    int brocheAnalogique;
    ZMPT101B* capteur;

public:
    CapteurTension(int pin);
    void initialiser();
    float lireValeurTension();
};

#endif
