#include "CapteurTemp.h"

/**
 * @brief Constructeur - initialise le bus OneWire, le capteur et configure le seuil d'alarme haute
 * @param seuilMax Température maximale en degrés Celsius
 */
CapteurTemp::CapteurTemp(int8_t seuilMax)
    : oneWire(PIN_DS18S20), capteur(&oneWire), sMax(seuilMax)
{
    capteur.begin();
    if (capteur.getDeviceCount() == 0)
    {
        if (DEBUGETTEST) Serial.println("CapteurTemp : aucun capteur détecté !");
    }
    else
    {
        DeviceAddress adresse;
        if (capteur.getAddress(adresse, 0))
        {
            capteur.setHighAlarmTemp(adresse, sMax);
        }
        else
        {
            if (DEBUGETTEST) Serial.println("CapteurTemp : erreur lecture adresse !");
        }
    }
}

/**
 * @brief Surveille la température et détecte un dépassement du seuil
 * @return true si le seuil de température est dépassé, false sinon ou si aucun capteur n'est détecté
 */
bool CapteurTemp::surveillerTemperature()
{
    bool retour = false;
    if (capteur.getDeviceCount() != 0)
    {
        capteur.requestTemperatures();
        retour = capteur.hasAlarm();
    }
    return retour;
}