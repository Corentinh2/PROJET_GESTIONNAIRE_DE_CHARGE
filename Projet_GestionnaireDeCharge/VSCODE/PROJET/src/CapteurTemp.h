#ifndef CAPTEUR_TEMP_H
#define CAPTEUR_TEMP_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/** @brief Broche de données du capteur DS18B20 */
#define PIN_DS18S20 18

/**
 * @brief Classe de gestion du capteur de température DS18B20
 */
class CapteurTemp {
private:
    /** @brief Bus OneWire pour la communication avec le capteur */
    OneWire oneWire;

    /** @brief Objet de gestion du capteur Dallas Temperature */
    DallasTemperature capteur;

    /** @brief Seuil maximum de température en degrés Celsius */
    int8_t sMax;

public:
    /**
     * @brief Constructeur - initialise le capteur et configure le seuil d'alarme
     * @param seuilMax Température maximale en degrés Celsius avant déclenchement de l'alarme
     */
    CapteurTemp(int8_t seuilMax);

    /**
     * @brief Vérifie si la température dépasse le seuil configuré
     * @return true si une alarme de température est détectée, false sinon
     */
    bool surveillerTemperature();
};

#endif