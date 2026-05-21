#ifndef HORLOGE_TEMPS_REEL_H
#define HORLOGE_TEMPS_REEL_H

#include <Wire.h>
#include <RTClib.h>
#include "constantes.h"


/**
 * @brief Classe de gestion de l'horloge temps réel DS3231
 *        Hérite de RTC_DS3231 et ajoute la gestion des alarmes et la synchronisation NTP
 */
class HorlogeTempsReel : public RTC_DS3231
{
private:
  /** @brief Indique si l'initialisation du DS3231 s'est effectuée correctement */
  bool succes;

  /**
   * @brief Gestionnaire d'interruption déclenché par l'alarme du DS3231
   *        Placé en RAM pour une exécution rapide
   */
  void IRAM_ATTR onAlarme();

  /**
   * @brief Fonction statique intermédiaire pour router l'interruption vers l'instance courante
   */
  static void marshall();

  /** @brief Pointeur statique vers l'instance courante pour le routage de l'interruption */
  static HorlogeTempsReel *anchor;

  /** @brief Indicateur volatile d'alarme déclenchée, modifié dans le contexte d'interruption */
  volatile bool alarmeDeclenchee;

public:
  /**
   * @brief Constructeur - initialise le DS3231 et configure l'interruption d'alarme
   */
  HorlogeTempsReel();

  /**
   * @brief Synchronise l'horloge temps réel avec un serveur NTP via WiFi
   */
  void synchroniserNTP();

  /**
   * @brief Retourne l'heure actuelle du DS3231
   * @return Objet DateTime contenant la date et l'heure courantes
   */
  DateTime obtenirHeureActuelle();

  /**
   * @brief Affiche la date et l'heure actuelles sur le port série
   */
  void afficherHeureAcutelle();

  /**
   * @brief Retourne l'état d'initialisation de l'horloge
   * @return true si le DS3231 est initialisé correctement, false sinon
   */
  const bool obtenirEtat();

  /**
   * @brief Configure une alarme se déclenchant toutes les minutes
   */
  void configurerAlarmeMinute();

  /**
   * @brief Réinitialise l'alarme et la replanifie 60 secondes plus tard
   */
  void reinitialiserAlarme();

  /**
   * @brief Retourne l'état de déclenchement de l'alarme
   * @return true si l'alarme a été déclenchée, false sinon
   */
  bool getAlarme();

  /**
   * @brief Retourne le jour de la semaine au format 1 (lundi) à 7 (dimanche)
   * @return Entier représentant le jour de la semaine
   */
  int obtenirJourSemaine();
};

#endif