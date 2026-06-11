#include "GestionnaireCharge.h"
#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief Constructeur - initialise tous les composants du gestionnaire de charge
 *        dans l'ordre : horloge, mémoire, relais, communication, capteur de température
 */
GestionnaireCharge::GestionnaireCharge(const char *_ssid, const char *_motDePasse) : energieCumuleeWh(0)
{
  ssid = _ssid;
  motDePasse = _motDePasse;
  derniereTentativeWifi = 0;

  horloge = new HorlogeTempsReel;
  if (DEBUGETTEST)
  {
    if (horloge->obtenirEtat())
    {
      Serial.println("HorlogeTempsReel : OK");
    }
    else
    {
      Serial.println("HorlogeTempsReel : PASOK");
    }
  }

  memoire = new MemoireProgramme;
  relais = new RelaisCommande;
  communication = new CommunicationMobile(memoire);
  ds18s20 = new CapteurTemp(TEMPMAX);

  raspi = new ConnectionRaspberryPi;
  capteur = new CapteurCourantTension;

  horloge->synchroniserNTP();
  horloge->configurerAlarmeMinute();

  chargeEnCourt = false;
  marcheForceeActive = false;
}

/**
 * @brief Destructeur - libère la mémoire de tous les composants
 */
GestionnaireCharge::~GestionnaireCharge()
{
  delete memoire;
  delete horloge;
  delete relais;
  delete communication;
  delete ds18s20;
  delete raspi;
  delete capteur;
}


/**
 * @brief Contrôle la charge en fonction du calendrier, de la marche forcée et de la température.
 *        Appelée à chaque tour de loop(), vérifie l'alarme minute, les messages WebSocket
 *        et la température du capteur.
 */
void GestionnaireCharge::controler()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    unsigned long maintenant = millis();
    if (maintenant - derniereTentativeWifi >= 10000)
    {
      if (DEBUGETTEST) Serial.println("WiFi perdu, tentative de reconnexion...");
      WiFi.disconnect();
      WiFi.begin(ssid, motDePasse);
      derniereTentativeWifi = maintenant;
    }
  }

  if (horloge->getAlarme())
  {
    horloge->reinitialiserAlarme();
    
    if(chargeEnCourt || marcheForceeActive)
    {
      raspi->EnvoyerMesures(capteur->getMoyennePuissance());
    } 

    DateTime maintenant = horloge->obtenirHeureActuelle();
    int jour = horloge->obtenirJourSemaine();
    int heure = maintenant.hour();
    int minute = maintenant.minute();
    bool debutTrouve = memoire->rechercherSession(jour, heure, minute, true);
    if (DEBUGETTEST) horloge->afficherHeureAcutelle();

    if (debutTrouve)
    {
      if (DEBUGETTEST) Serial.println("Début de créneau → action début !");
      relais->fermer();
      chargeEnCourt = true;
    }
    else
    {
      bool finTrouve = memoire->rechercherSession(jour, heure, minute, false);
      if (finTrouve)
      {
        if (DEBUGETTEST) Serial.println("Fin de créneau → action fin !");
        relais->ouvrir();
        chargeEnCourt = false;
      }
    }
  }

  int marcheForcee = communication->gererCommunication();
  if (marcheForcee == 1)
  {
    if (DEBUGETTEST) Serial.println("Marche forcée activée");
    relais->fermer();
    marcheForceeActive = true;
  }
  if (marcheForcee == 0)
  {
    if (DEBUGETTEST) Serial.println("Marche forcée désactivée");
    marcheForceeActive = false;
    if (!chargeEnCourt)
    {
      relais->ouvrir();
    }
    else
    {
      if (DEBUGETTEST) Serial.println("Retour au cycle de charge planifié.");
    }
  }

  if (chargeEnCourt || marcheForceeActive)
  {
    capteur->effectuerMesures();
    if(ds18s20->surveillerTemperature())
    {
      if (DEBUGETTEST) Serial.println("ALERTE TEMP");
      relais->ouvrir();
      memoire->ajouterAlerte(true);
      raspi->EnvoyerAlerte(1, "Surchauffe detecte");
      chargeEnCourt = false;
      marcheForceeActive = false;
    }
    if (capteur->verifierSeuils())
    {
      relais->ouvrir();
      memoire->ajouterAlerte(false);
      raspi->EnvoyerAlerte(0, "SurIntensite detecte");
      chargeEnCourt = false;
      marcheForceeActive = false;
    }
  }
}


// JUSTE POUR LE TEST UNITAIRE
MemoireProgramme *GestionnaireCharge::obtenirMemoire()
{
  return memoire;
}

// JUSTE POUR LE TEST UNITAIRE
HorlogeTempsReel *GestionnaireCharge::obtenirHorloge()
{
  return horloge;
}
