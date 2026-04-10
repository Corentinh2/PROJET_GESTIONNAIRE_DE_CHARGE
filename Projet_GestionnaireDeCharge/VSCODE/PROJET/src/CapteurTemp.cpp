#include "CapteurTemp.h"

CapteurTemp::CapteurTemp(int8_t seuilMax)
    : oneWire(PIN_DS18S20), capteur(&oneWire), sMax(seuilMax)
{
    capteur.begin();

    if (capteur.getDeviceCount() == 0)
    {
        Serial.println("CapteurTemp : aucun capteur détecté !");
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
            Serial.println("CapteurTemp : erreur lecture adresse !");
        }
    }
}

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
