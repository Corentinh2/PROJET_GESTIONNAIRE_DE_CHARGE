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
  {
    etat = true;
  }
  relais = new RelaisCommande;
  communication = new CommunicationMobile(memoire);

  ds18s20 = new CapteurTemp(TEMPMAX);

  horloge->syncFromNTP();
  horloge->configurerAlarmeMinute();

  chargeEnCourt = false;
  marcheForceeActive = false;
}

GestionnaireCharge::~GestionnaireCharge()
{
  delete memoire;
  delete horloge;
  delete relais;
  delete communication;
  delete ds18s20;
}

bool GestionnaireCharge::obtenirEtat() const
{
  return etat;
}

void GestionnaireCharge::syncroniserHorloge()
{
  horloge->syncFromNTP();
}

void GestionnaireCharge::controler()
{
  if (horloge->getAlarme())
  {
    horloge->reinitialiserAlarme();

    DateTime maintenant = horloge->getTime();
    int jour = horloge->obtenirJourSemaine();
    int heure = maintenant.hour();
    int minute = maintenant.minute();
    bool debutTrouve = memoire->lireCalendrier(jour, heure, minute, true);

    horloge->printTime();
    if (debutTrouve)
    {
      Serial.println("Début de créneau → action début !");
      relais->fermer();
      chargeEnCourt = true;
    }
    else
    {
      if (memoire->lireCalendrier(jour, heure, minute, false))
      {
        Serial.println("Fin de créneau → action fin !");
        relais->ouvrir();
        chargeEnCourt = false;
      }
    }
  }

  int marcheForcee = communication->gererCommunication();
  if (marcheForcee == 1)
  {
    Serial.println("Marche forcée activée");
    relais->fermer();
    marcheForceeActive = true;
    // chargeEnCourt = true;
  }
  if (marcheForcee == 0)
  {
    Serial.println("Marche forcée désactivée");
    marcheForceeActive = false;
    if (!chargeEnCourt)
    {
      relais->ouvrir();
    }
    else
    {
      Serial.println("Retour au cycle de charge planifié.");
    }
  }


  if ((chargeEnCourt || marcheForceeActive) && ds18s20->surveillerTemperature())
  {
    Serial.println("ALERTE TEMP");
    relais->ouvrir();
    memoire->ajouterAlerte(true);
    chargeEnCourt = false;
    marcheForceeActive = false; 
  }
}