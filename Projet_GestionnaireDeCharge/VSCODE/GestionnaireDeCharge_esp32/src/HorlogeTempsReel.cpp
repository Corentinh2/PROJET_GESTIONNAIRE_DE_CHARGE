#include "HorlogeTempsReel.h"
#include <Arduino.h>
#include <time.h>

/**
 * @brief Constructeur - initialise le DS3231, configure la broche d'alarme
 *        et attache l'interruption sur front descendant
 */
HorlogeTempsReel::HorlogeTempsReel()
{
  succes = false;
  alarmeDeclenchee = false;
  pinMode(PIN_ALARME, INPUT_PULLUP);
  anchor = this;
  if (RTC_DS3231::begin())
  {
    Serial.println("DS3231 initialisé.");
    succes = true;
    attachInterrupt(digitalPinToInterrupt(PIN_ALARME), HorlogeTempsReel::marshall, FALLING);
  }
  else
  {
    Serial.println("DS3231 introuvable !");
  }
}

/**
 * @brief Synchronise l'horloge temps réel avec un serveur NTP
 *        Configure le fuseau horaire Europe/Paris (CET/CEST)
 *        Effectue jusqu'à 20 tentatives de synchronisation
 */
void HorlogeTempsReel::synchroniserNTP()
{
  bool ntpOk = false;
  struct tm timeinfo;
  int essaies = 0;

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();

  while (!getLocalTime(&timeinfo) && essaies < 20)
  {
    Serial.print(".");
    delay(500);
    essaies++;
  }

  if (essaies < 20)
  {
    ntpOk = true;
  }

  if (ntpOk)
  {
    adjust(DateTime(
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec));
    Serial.println("\nDS3231 synchronisé !");
  }
  else
  {
    Serial.println("\nÉchec NTP !");
  }
}

/**
 * @brief Retourne l'heure actuelle du DS3231
 * @return Objet DateTime contenant la date et l'heure courantes
 */
DateTime HorlogeTempsReel::obtenirHeureActuelle()
{
  return now();
}

/**
 * @brief Affiche la date et l'heure actuelles sur le port série
 *        Format : JJ/MM/AAAA HH:MM:SS
 */
void HorlogeTempsReel::afficherHeureAcutelle()
{
  DateTime maintenant = now();
  Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
                maintenant.day(), maintenant.month(), maintenant.year(),
                maintenant.hour(), maintenant.minute(), maintenant.second());
}

/**
 * @brief Retourne l'état d'initialisation de l'horloge
 * @return true si le DS3231 est initialisé correctement, false sinon
 */
const bool HorlogeTempsReel::obtenirEtat()
{
  return succes;
}

/**
 * @brief Configure une alarme se déclenchant toutes les minutes
 *        Désactive le signal carré et active l'interruption via INTCN
 */
void HorlogeTempsReel::configurerAlarmeMinute()
{
  disable32K();
  clearAlarm(1);
  clearAlarm(2);
  disableAlarm(2);
  writeSqwPinMode(DS3231_OFF);

  if (!setAlarm1(now() + TimeSpan(60), DS3231_A1_Second))
  {
    Serial.println("Erreur configuration alarme !");
  }
  else
  {
    Serial.println("Alarme minute configurée !");
  }
}

/**
 * @brief Réinitialise l'alarme et la replanifie 60 secondes plus tard
 *        Remet également le drapeau alarmeDeclenchee à false
 */
void HorlogeTempsReel::reinitialiserAlarme()
{
  clearAlarm(1);
  setAlarm1(now() + TimeSpan(60), DS3231_A1_Second);
  alarmeDeclenchee = false;
}

/**
 * @brief Retourne l'état de déclenchement de l'alarme
 * @return true si l'alarme a été déclenchée, false sinon
 */
bool HorlogeTempsReel::getAlarme()
{
  return alarmeDeclenchee;
}

/**
 * @brief Retourne le jour de la semaine au format 1 (lundi) à 7 (dimanche)
 *        Convertit le format du DS3231 (0 = dimanche) vers le format ISO (7 = dimanche)
 * @return Entier représentant le jour de la semaine
 */
int HorlogeTempsReel::obtenirJourSemaine()
{
  int jourRTC = now().dayOfTheWeek();
  int jourConverti = 0;
  if (jourRTC == 0)
  {
    jourConverti = 7;
  }
  else
  {
    jourConverti = jourRTC;
  }
  return jourConverti;
}

/**
 * @brief Fonction statique intermédiaire pour router l'interruption vers l'instance courante
 *        Nécessaire car attachInterrupt ne peut pas prendre une méthode non statique
 */
void HorlogeTempsReel::marshall()
{
  anchor->onAlarme();
}

/**
 * @brief Gestionnaire d'interruption déclenché sur front descendant de PIN_ALARME
 *        Placé en RAM (IRAM_ATTR) pour une exécution rapide
 */
void IRAM_ATTR HorlogeTempsReel::onAlarme()
{
  alarmeDeclenchee = true;
}

/** @brief Initialisation du pointeur statique vers l'instance courante */
HorlogeTempsReel *HorlogeTempsReel::anchor = NULL;