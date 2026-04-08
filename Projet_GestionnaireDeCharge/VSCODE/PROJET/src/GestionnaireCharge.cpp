#include "GestionnaireCharge.h"
#include <Arduino.h>

GestionnaireCharge::GestionnaireCharge()
{

  horloge = new HorlogeTempsReel;
  if (horloge->obtenirEtat())
  {
    Serial.println("HorlogeTempsReel : OK");
  }
  else
  {
    Serial.println("HorlogeTempsReel : PASOK");
  }
  memoire = new MemoireProgramme;
  if (memoire->obtenirEtat())
  {
    Serial.println("memoire : OK");
  }
  else
  {
    Serial.println("memoire : PASOK");
  }

  if (memoire->obtenirEtat() && horloge->obtenirEtat())
    etat = true;
  relais = new RelaisCommande;
  communication = new CommunicationMobile(memoire, relais);

  horloge->syncFromNTP();
  horloge->configurerAlarmeMinute();

}

GestionnaireCharge::~GestionnaireCharge()
{
  delete memoire;
  delete horloge;
  delete relais;
  delete communication;
}

MemoireProgramme *GestionnaireCharge::obtenirMemoire()
{
  return memoire;
}

HorlogeTempsReel *GestionnaireCharge::obtenirHorloge()
{
  return horloge;
}


bool GestionnaireCharge::obtenirEtat() const
{
  return etat;
}

void GestionnaireCharge::syncroniserHorloge()
{
  horloge->syncFromNTP();
}

void GestionnaireCharge::interrogerCalendrier()
{
  if (obtenirHorloge()->getAlarme())
  {
    obtenirHorloge()->reinitialiserAlarme();

    DateTime maintenant = obtenirHorloge()->getTime();
    int jour = obtenirHorloge()->obtenirJourSemaine();
    int heure = maintenant.hour();
    int minute = maintenant.minute();
    bool debutTrouve = obtenirMemoire()->lireCalendrier(jour, heure, minute, true);

   obtenirHorloge()->printTime();
    if (debutTrouve)
    {
      Serial.println("Début de créneau → action début !");
      relais->fermer();
    }

    if (!debutTrouve)
    {
      if (obtenirMemoire()->lireCalendrier(jour, heure, minute, false))
      {
        Serial.println("Fin de créneau → action fin !");
        relais->ouvrir();
      }
    }
  }
}

void GestionnaireCharge::gererCommunication() {
  communication->gerer();
}