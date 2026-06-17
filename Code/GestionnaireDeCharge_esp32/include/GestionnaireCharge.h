#ifndef GESTIONNAIRE_CHARGE_H
#define GESTIONNAIRE_CHARGE_H

#include "HorlogeTempsReel.h"
#include "MemoireProgramme.h"
#include "RelaisCommande.h"
#include "CommunicationMobile.h"
#include "CapteurTemp.h"
#include "ConnectionRaspberryPi.h"
#include "CapteurCourantTension.h"
#include "constantes.h"

/**
 * @brief Classe principale de gestion de la charge du véhicule électrique
 */
class GestionnaireCharge
{
private:
  /** @brief Pointeur vers l'horloge temps réel DS3231 */
  HorlogeTempsReel *horloge;

  /** @brief Pointeur vers la mémoire programme (base de données SQLite) */
  MemoireProgramme *memoire;

  /** @brief Pointeur vers le relais de commande de la charge */
  RelaisCommande *relais;

  /** @brief Pointeur vers le gestionnaire de communication WebSocket */
  CommunicationMobile *communication;

  /** @brief Pointeur vers le capteur de température DS18B20 */
  CapteurTemp *ds18s20;

  ConnectionRaspberryPi  *raspi;
  CapteurCourantTension  *capteur;

  /** @brief Indique si tous les composants sont initialisés correctement */
  bool etat;

  /** @brief Indique si une charge est en cours */
  bool chargeEnCourt;

  /** @brief Indique si la marche forcée est active */
  bool marcheForceeActive;

  /** @brief Pointeur vers le SSID du réseau WiFi */
  const char *ssid;

  /** @brief Pointeur vers le mot de passe du réseau WiFi */
  const char *motDePasse;

  /** @brief Timestamp de la dernière tentative de reconnexion WiFi */
  unsigned long derniereTentativeWifi;

  float energieCumuleeWh;

public:
  /**
   * @brief Constructeur - initialise tous les composants du gestionnaire de charge
   */
  GestionnaireCharge(const char *ssid, const char *motDePasse);

  /**
   * @brief Destructeur - libère la mémoire de tous les composants
   */
  ~GestionnaireCharge();

  /**
   * @brief Contrôle la charge en fonction du calendrier, de la marche forcée et de la température
   */
  void controler();

  // JUSTE POUR LE TEST UNITAIRE
  MemoireProgramme *obtenirMemoire();
  HorlogeTempsReel *obtenirHorloge();
};

#endif