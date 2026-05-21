#ifndef MEMOIRE_PROGRAMME_H
#define MEMOIRE_PROGRAMME_H

#include <Arduino.h>
#include "constantes.h"

typedef struct sqlite3 sqlite3;

/**
 * @brief Classe de gestion de la mémoire programme via une base de données SQLite
 *        Stocke les créneaux de charge (calendrier) et les alertes sur LittleFS
 */
class MemoireProgramme
{
private:
  /** @brief Pointeur vers la base de données SQLite */
  sqlite3 *db;

  /** @brief Indique si la base de données est ouverte et opérationnelle */
  bool estOuverte;

  /**
   * @brief Crée les tables CALENDRIER et ALERTE si elles n'existent pas
   * @return true si la création des tables a réussi, false sinon
   */
  bool creerTables();

  /**
   * @brief Calcule le prochain identifiant de calendrier disponible
   * @return Prochain identifiant entier disponible
   */
  int obtenirProchainIdCalendrier();

public:
  /**
   * @brief Constructeur - monte LittleFS, ouvre la base de données et crée les tables
   */
  MemoireProgramme();

  /**
   * @brief Destructeur - ferme la base de données proprement
   */
  ~MemoireProgramme();

  /**
   * @brief Ajoute un événement de charge dans le calendrier
   *        Gère automatiquement les créneaux à cheval sur minuit
   * @param _jours Masque de bits des jours de la semaine (bits 0 à 6 = lundi à dimanche)
   * @param _hd Heure de début (0-23)
   * @param _md Minute de début (0 ou 30)
   * @param _hf Heure de fin (0-23)
   * @param _mf Minute de fin (0 ou 30)
   * @return true si l'ajout a réussi, false sinon
   */
  bool ajouterEvenement(int _jours, int _hd, int _md, int _hf, int _mf);

  /**
   * @brief Insère une ligne dans la table CALENDRIER
   * @param _id Identifiant du calendrier
   * @param _jours Jour de la semaine (1 à 7)
   * @param _hd Heure de début (0-23)
   * @param _md Minute de début (0 ou 30)
   * @param _hf Heure de fin (0-23)
   * @param _mf Minute de fin (0 ou 30)
   * @return true si l'insertion a réussi, false sinon
   */
  bool insererCalendrier(int _id, int _jours, int _hd, int _md, int _hf, int _mf);

  /**
   * @brief Vérifie si un créneau correspond à l'heure actuelle
   * @param _jours Jour de la semaine (1 à 7)
   * @param _heure Heure courante (0-23)
   * @param _minute Minute courante (0 ou 30)
   * @param _debut true pour chercher un début de créneau, false pour une fin
   * @return true si un créneau correspondant est trouvé, false sinon
   */
  bool rechercherSession(int _jours, int _heure, int _minute, bool _debut);

  /**
   * @brief Supprime tous les enregistrements d'un calendrier par son identifiant
   * @param _id Identifiant du calendrier à supprimer
   * @return true si la suppression a réussi, false sinon
   */
  bool supprimerCalendrier(int _id);

  /**
   * @brief Décode un masque de bits de jours en tableau de jours individuels
   * @param _joursRecu Masque de bits des jours (bits 0 à 6 = lundi à dimanche)
   * @param _joursDecodes Tableau de sortie des jours décodés, terminé par -1
   * @return true si au moins un jour valide a été décodé, false sinon
   */
  bool decoderJoursTrame(int _joursRecu, int *_joursDecodes);

  /**
   * @brief Ajoute une alerte dans la table ALERTE
   * @param _type Type de l'alerte (true = surchauffe, false = surintensité)
   * @return true si l'ajout a réussi, false sinon
   */
  bool ajouterAlerte(bool _type);

  /**
   * @brief Supprime toutes les alertes de la table ALERTE
   * @return true si la suppression a réussi, false sinon
   */
  bool supprimerAlerte();

  /**
   * @brief Retourne les créneaux du calendrier au format JSON
   * @return String contenant les trames JSON des calendriers
   */
  String obtenirTrameOriginale();

  /**
   * @brief Retourne les alertes au format JSON
   * @return String contenant la trame JSON de l'alerte courante
   */
  String obtenirAlerte();



  // JUSTE POUR LE TEST UNITAIRE
  void afficherCalendrierTestUnitaire();
  // JUSTE POUR LE TEST UNITAIRE
  void afficherAlerteTestUnitaire();
};

#endif