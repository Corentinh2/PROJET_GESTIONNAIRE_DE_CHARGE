#include "GestionnaireCharge.h"
#include "esp32_snir.h"
#include <Arduino.h>
#include <WiFi.h>

/**
 * @brief Constructeur - initialise tous les composants du gestionnaire de charge
 *        dans l'ordre : horloge, mémoire, relais, communication, capteur de température
 */
GestionnaireCharge::GestionnaireCharge(const char *_ssid, const char *_motDePasse)
    : sommeTension(0), sommeCourant(0),
      energieCumuleeWh(0), nombreLectures(0), chronoMinute(0)
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

  capteurCourant = new CapteurCourant(COURANT, 1);
  capteurTension = new CapteurTension(TENSION);
  raspi = new ConnectionRaspberryPi();

  horloge->synchroniserNTP();
  horloge->configurerAlarmeMinute();
  raspi->initialiserConnexion();
  initialiserCapteurs();

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
  delete capteurCourant;
  delete capteurTension;
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

void GestionnaireCharge::initialiserCapteurs()
{
  capteurCourant->initialiser();
  capteurTension->initialiser();
  chronoMinute = millis();
}

void GestionnaireCharge::envoyerMesures()
{
  // On englobe tout dans un bloc if pour éviter le return prématuré
  if (millis() - derniereMesure >= 1000)
  {
    derniereMesure = millis();

    float V = capteurTension->lireValeurTension();
    float I = capteurCourant->lireValeurCourant();
    float P = V * I;

    Serial.println("Tension (V)\t| Courant (A)\t| Puissance (W)");
    Serial.println("----------------|---------------|---------------");
    Serial.print(V, 1);
    Serial.print(" V\t| ");
    Serial.print(I, 2);
    Serial.print(" A\t| ");
    Serial.print(P, 1);
    Serial.println(" W");
    Serial.println("------------------------------------------------");

    sommeTension += V;
    sommeCourant += I;
    nombreLectures++;

    if (millis() - chronoMinute >= 60000)
    {
      float moyV = sommeTension / nombreLectures;
      float moyI = sommeCourant / nombreLectures;
      float moyP = moyV * moyI;
      energieCumuleeWh += (moyP / 60.0);

      Serial.println("\n--- RAPPORT DE LA DERNIERE MINUTE ---");
      Serial.println("U Moy (V) | I Moy (A) | P Moy (W) | Conso (Wh)");
      Serial.print(moyV, 1);
      Serial.print("\t| ");
      Serial.print(moyI, 2);
      Serial.print("\t| ");
      Serial.print(moyP, 1);
      Serial.print("\t| ");
      Serial.print(energieCumuleeWh, 3);
      Serial.println(" Wh");

      String trame = "{\"puissance\":" + String(moyP, 2) + "}";
      Serial.println("[WS] Envoi trame : " + trame);
      raspi->EnvoyerNotification(trame);

      sommeTension = 0;
      sommeCourant = 0;
      nombreLectures = 0;
      chronoMinute = millis();
    }
  }
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
      bool finTrouve = memoire->rechercherSession(jour, heure, minute, false);
      if (finTrouve)
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
    memoire->ajouterAlerte(true); // 0 pour surchauffe
    chargeEnCourt = false;
    marcheForceeActive = false;
    raspi->EnvoyerAlerte(1, "Surchauffe detectee");
  }

  if ((chargeEnCourt || marcheForceeActive) && capteurCourant->surveillerCourant())
  {
    relais->ouvrir();
    memoire->ajouterAlerte(false); // 0 pour surintensité
    chargeEnCourt = false;
    marcheForceeActive = false;
    raspi->EnvoyerAlerte(0, "Surintensite detectee");
  }

  raspi->maintenirConnexion();
  envoyerMesures();
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