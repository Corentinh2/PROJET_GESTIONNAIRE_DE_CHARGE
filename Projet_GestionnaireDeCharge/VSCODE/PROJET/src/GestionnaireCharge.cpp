#include "GestionnaireCharge.h"
#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief Constructeur - initialise tous les composants du gestionnaire de charge
 *        dans l'ordre : horloge, mémoire, relais, communication, capteur de température
 */
GestionnaireCharge::GestionnaireCharge(const char *_ssid, const char *_motDePasse)
{
  ssid = _ssid;
  motDePasse = _motDePasse;
  derniereTentativeWifi = 0;

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
}

/**
 * @brief Retourne l'état d'initialisation du gestionnaire
 * @return true si tous les composants sont initialisés correctement, false sinon
 */
bool GestionnaireCharge::obtenirEtat() const
{
  return etat;
}

/**
 * @brief Synchronise l'horloge temps réel avec un serveur NTP
 */
void GestionnaireCharge::synchroniserHorloge()
{
  horloge->synchroniserNTP();
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
      Serial.println("WiFi perdu, tentative de reconnexion...");
      WiFi.disconnect();
      WiFi.begin(ssid, motDePasse);
      derniereTentativeWifi = maintenant;
    }
  }
  if (horloge->getAlarme())
  {
    horloge->reinitialiserAlarme();
    DateTime maintenant = horloge->obtenirHeureActuelle();
    int jour = horloge->obtenirJourSemaine();
    int heure = maintenant.hour();
    int minute = maintenant.minute();
    bool debutTrouve = memoire->rechercherSession(jour, heure, minute, true);
    horloge->afficherHeureAcutelle();

    if (debutTrouve)
    {
      Serial.println("Début de créneau → action début !");
      relais->fermer();
      chargeEnCourt = true;
    }
    else
    {
      if (memoire->rechercherSession(jour, heure, minute, false))
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