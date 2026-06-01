#include "MemoireProgramme.h"
#include <LittleFS.h>
#include <sqlite3.h>
#include <Arduino.h>

/**
 * @brief Constructeur - monte LittleFS, ouvre la base de données SQLite
 *        et crée les tables si elles n'existent pas
 */
MemoireProgramme::MemoireProgramme()
{
  db = nullptr;
  estOuverte = false;

  if (LittleFS.begin(true))
  {
    if (sqlite3_open(CHEMIN_BDD, &db) == SQLITE_OK)
    {
      if (creerTables())
      {
        estOuverte = true;
      }
      else
      {
        if (DEBUGETTEST) Serial.println("MemoireProgramme : Erreur création tables !");
      }
    }
    else
    {
      if (DEBUGETTEST) Serial.printf("MemoireProgramme : Erreur BDD : %s\n", sqlite3_errmsg(db));
    }
  }
  else
  {
    if (DEBUGETTEST) Serial.println("MemoireProgramme : Erreur montage LittleFS !");
  }
}

/**
 * @brief Destructeur - ferme la base de données SQLite proprement
 */
MemoireProgramme::~MemoireProgramme()
{
  if (estOuverte)
  {
    sqlite3_close(db);
    estOuverte = false;
  }
}

/**
 * @brief Crée les tables SESSIONS et ALERTES si elles n'existent pas
 * @return true si la création des tables a réussi, false sinon
 */
bool MemoireProgramme::creerTables()
{
  bool succes = true;
  char *errMsg = nullptr;

  const char *requeteSession =
      "CREATE TABLE IF NOT EXISTS SESSIONS ("
      "id_session     INTEGER PRIMARY KEY AUTOINCREMENT,"
      "num_calendrier INTEGER,"
      "jours          INTEGER,"
      "hd             INTEGER,"
      "md             INTEGER,"
      "hf             INTEGER,"
      "mf             INTEGER"
      ");";

  const char *requeteAlerte =
      "CREATE TABLE IF NOT EXISTS ALERTES ("
      "id_alerte   INTEGER PRIMARY KEY,"
      "type_alerte BOOLEAN"
      ");";

  if (sqlite3_exec(db, requeteSession, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur table SESSIONS : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes && sqlite3_exec(db, requeteAlerte, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur table ALERTES : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  return succes;
}

/**
 * @brief Calcule le prochain numéro de calendrier disponible
 * @return Prochain numéro entier disponible (MAX + 1, ou 1 si vide)
 */
int MemoireProgramme::obtenirProchainNumCalendrier()
{
  sqlite3_stmt *stmt = nullptr;
  int prochain = 1;

  const char *requete = "SELECT COALESCE(MAX(num_calendrier), 0) + 1 FROM SESSIONS;";

  if (sqlite3_prepare_v2(db, requete, -1, &stmt, nullptr) == SQLITE_OK)
  {
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
      prochain = sqlite3_column_int(stmt, 0);
    }
  }

  sqlite3_finalize(stmt);
  return prochain;
}

/**
 * @brief Ajoute un calendrier dans la table SESSIONS
 *        Gère automatiquement les créneaux à cheval sur minuit en insérant deux lignes
 * @param _joursRecu Masque de bits des jours (bits 0 à 6 = lundi à dimanche)
 * @param _hd Heure de début (0-23)
 * @param _md Minute de début (0 ou 30)
 * @param _hf Heure de fin (0-23)
 * @param _mf Minute de fin (0 ou 30)
 * @return true si l'ajout a réussi, false sinon
 */
bool MemoireProgramme::ajouterCalendrier(int _joursRecu, int _hd, int _md, int _hf, int _mf)
{
  bool succes = true;
  int joursDecodes[8];
  bool aCheval = (_hf < _hd);

  succes = decoderJoursTrame(_joursRecu, joursDecodes);

  if (succes)
  {
    int numCalendrier = obtenirProchainNumCalendrier();

    for (int i = 0; joursDecodes[i] != -1 && succes; i++)
    {
      int jourActuel = joursDecodes[i];
      int jourSuivant = jourActuel + 1;

      if (jourActuel == 7)
      {
        jourSuivant = 1;
      }

      if (aCheval)
      {
        succes = insererSession(numCalendrier, jourActuel, _hd, _md, 23, 59);
        if (succes)
        {
          succes = insererSession(numCalendrier, jourSuivant, 0, 0, _hf, _mf);
        }
      }

      if (!aCheval)
      {
        succes = insererSession(numCalendrier, jourActuel, _hd, _md, _hf, _mf);
      }
    }
  }

  if (DEBUGETTEST)
  {
    if (succes)
    {
      Serial.printf("Calendrier %d ajouté avec succès !\n", obtenirProchainNumCalendrier() - 1);
    }
  }
  return succes;
}

/**
 * @brief Insère une session de charge dans la table SESSIONS
 * @param _num Numéro du calendrier
 * @param _jours Jour de la semaine (1 à 7)
 * @param _hd Heure de début (0-23)
 * @param _md Minute de début (0 ou 30)
 * @param _hf Heure de fin (0-23)
 * @param _mf Minute de fin (0 ou 30)
 * @return true si l'insertion a réussi, false sinon
 */
bool MemoireProgramme::insererSession(int _num, int _jours, int _hd, int _md, int _hf, int _mf)
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[256];

  snprintf(requete, sizeof(requete),
           "INSERT INTO SESSIONS (num_calendrier, jours, hd, md, hf, mf) "
           "VALUES (%d, %d, %d, %d, %d, %d);",
           _num, _jours, _hd, _md, _hf, _mf);

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur insertion session : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  return succes;
}

static int callbackCalendrier(void *data, int nbColonnes, char **valeurs, char **nomColonnes)
{
  bool *trouve = (bool *)data;
  *trouve = true;
  return 0;
}

/**
 * @brief Vérifie si une session correspond à l'heure actuelle
 * @param _jours Jour de la semaine (1 à 7)
 * @param _heure Heure courante (0-23)
 * @param _minute Minute courante (0 ou 30)
 * @param _debut true pour chercher un début de session, false pour une fin
 * @return true si une session correspondante est trouvée, false sinon
 */
bool MemoireProgramme::rechercherSession(int _jours, int _heure, int _minute, bool _debut)
{
  bool succes = true;
  bool trouve = false;
  char *errMsg = nullptr;
  char requete[256];

  if (_debut)
  {
    snprintf(requete, sizeof(requete),
             "SELECT * FROM SESSIONS WHERE jours = %d AND hd = %d AND md = %d;",
             _jours, _heure, _minute);
  }
  else
  {
    snprintf(requete, sizeof(requete),
             "SELECT * FROM SESSIONS WHERE jours = %d AND hf = %d AND mf = %d;",
             _jours, _heure, _minute);
  }

  if (sqlite3_exec(db, requete, callbackCalendrier, &trouve, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur lecture session : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes)
  {
    succes = trouve;
  }

  return succes;
}

/**
 * @brief Supprime toutes les sessions d'un calendrier par son numéro
 * @param _id Numéro du calendrier à supprimer
 * @return true si la suppression a réussi, false sinon
 */
bool MemoireProgramme::supprimerCalendrier(int _num)
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[128];

  snprintf(requete, sizeof(requete),
           "DELETE FROM SESSIONS WHERE num_calendrier = %d;", _num);

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur suppression SESSIONS : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (DEBUGETTEST)
  {
    if (succes)
    {
      Serial.printf("Calendrier %d supprimé !\n", _num);
    }
  }
  return succes;
}

/**
 * @brief Décode un masque de bits de jours en tableau de jours individuels
 * @param _joursRecu Masque de bits des jours (bits 0 à 6 = lundi à dimanche)
 * @param _joursDecodes Tableau de sortie des jours décodés, terminé par -1
 * @return true si au moins un jour valide a été décodé, false sinon
 */
bool MemoireProgramme::decoderJoursTrame(int _joursRecu, int *_joursDecodes)
{
  bool succes = true;
  int index = 0;

  for (int i = 0; i < 7; i++)
  {
    if (_joursRecu & (1 << i))
    {
      _joursDecodes[index] = i + 1;
      index++;
    }
  }

  _joursDecodes[index] = -1;

  if (index == 0)
  {
    if (DEBUGETTEST) Serial.println("Aucun jour valide recu !");
    succes = false;
  }

  return succes;
}

/**
 * @brief Ajoute une alerte dans la table ALERTES
 * @param _type Type de l'alerte (true = surchauffe, false = surintensité)
 * @return true si l'ajout a réussi, false sinon
 */
bool MemoireProgramme::ajouterAlerte(bool _type)
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[256];

  snprintf(requete, sizeof(requete),
           "INSERT INTO ALERTES (type_alerte) VALUES (%d);", _type);

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur insertion alerte : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  return succes;
}

/**
 * @brief Supprime toutes les alertes de la table ALERTES
 * @return true si la suppression a réussi, false sinon
 */
bool MemoireProgramme::supprimerAlerte()
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[128];

  snprintf(requete, sizeof(requete), "DELETE FROM ALERTES;");

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur suppression alerte : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (DEBUGETTEST)
  {
    if (succes)
    {
      Serial.println("table alerte vidée");
    }
  }

  return succes;
}

/**
 * @brief Retourne les créneaux du calendrier au format JSON
 *        Reconstitue les créneaux à cheval sur minuit en une seule trame
 * @return String contenant les trames JSON des calendriers
 */
String MemoireProgramme::obtenirTrameOriginale()
{
  String json = "";
  sqlite3_stmt *stmt = nullptr;

  const char *requete =
      "SELECT num_calendrier, jours, hd, md, hf, mf "
      "FROM SESSIONS "
      "ORDER BY num_calendrier, jours;";

  if (sqlite3_prepare_v2(db, requete, -1, &stmt, nullptr) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur préparation : %s\n", sqlite3_errmsg(db));
  }
  else
  {
    int idCourant = -1;
    int masqueJoursTous = 0;
    int masqueJoursStart = 0;
    int hd = 0, md = 0, hf = 0, mf = 0;
    bool premiereLigne = true;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      int numCalendrier = sqlite3_column_int(stmt, 0);
      int jour          = sqlite3_column_int(stmt, 1);
      int hdLigne       = sqlite3_column_int(stmt, 2);
      int mdLigne       = sqlite3_column_int(stmt, 3);
      int hfLigne       = sqlite3_column_int(stmt, 4);
      int mfLigne       = sqlite3_column_int(stmt, 5);

      if (numCalendrier != idCourant)
      {
        if (idCourant != -1)
        {
          int joursFinal;
          if (hd > hf)
          {
            joursFinal = masqueJoursStart;
          }
          else
          {
            joursFinal = masqueJoursTous;
          }

          char ligne[256];
          snprintf(ligne, sizeof(ligne),
                   "{\"action\":\"calendrier\",\"id\":%d,\"jours\":%d,\"hd\":%d,\"md\":%d,\"hf\":%d,\"mf\":%d}",
                   idCourant, joursFinal, hd, md, hf, mf);
          json += String(ligne) + "\n";
        }

        idCourant = numCalendrier;
        masqueJoursTous = 0;
        masqueJoursStart = 0;
        premiereLigne = true;
      }

      masqueJoursTous = masqueJoursTous | (1 << (jour - 1));

      if (hdLigne != 0 || mdLigne != 0)
      {
        masqueJoursStart |= (1 << (jour - 1));
      }

      if (premiereLigne)
      {
        hd = hdLigne;
        md = mdLigne;
        hf = hfLigne;
        mf = mfLigne;
        premiereLigne = false;
      }

      if (hfLigne == 23 && mfLigne == 59)
      {
        hd = hdLigne;
        md = mdLigne;
      }

      if (hdLigne == 0 && mdLigne == 0)
      {
        hf = hfLigne;
        mf = mfLigne;
      }
    }

    if (idCourant != -1)
    {
      int joursFinal;
      if (hd > hf)
      {
        joursFinal = masqueJoursStart;
      }
      else
      {
        joursFinal = masqueJoursTous;
      }

      char ligne[256];
      snprintf(ligne, sizeof(ligne),
               "{\"action\":\"calendrier\",\"id\":%d,\"jours\":%d,\"hd\":%d,\"md\":%d,\"hf\":%d,\"mf\":%d}",
               idCourant, joursFinal, hd, md, hf, mf);
      json += String(ligne) + "\n";
    }

    if (json.length() == 0)
    {
      json = "{\"action\":\"calendrier\",\"id\":0,\"jours\":0,\"hd\":0,\"md\":0,\"hf\":0,\"mf\":0}\n";
    }

    sqlite3_finalize(stmt);
    if (DEBUGETTEST) Serial.println(json);
  }

  return json;
}

/**
 * @brief Retourne les alertes stockées au format JSON
 *        Chaque alerte est retournée sous forme d'une trame JSON par ligne
 * @return String contenant les trames JSON des alertes
 */
String MemoireProgramme::obtenirAlerte()
{
  String json = "";
  sqlite3_stmt *stmt = nullptr;

  const char *requete = "SELECT type_alerte FROM ALERTES;";

  if (sqlite3_prepare_v2(db, requete, -1, &stmt, nullptr) != SQLITE_OK)
  {
    if (DEBUGETTEST) Serial.printf("Erreur préparation ALERTES : %s\n", sqlite3_errmsg(db));
  }
  else
  {
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      int typeBrut = sqlite3_column_int(stmt, 0);
      const char *typeAlerte;

      if (typeBrut == 1)
      {
        typeAlerte = "surchauffe";
      }
      else
      {
        typeAlerte = "surtension";
      }

      char ligne[256];
      snprintf(ligne, sizeof(ligne),
               "{\"action\":\"alerte\",\"type\":\"%s\"}",
               typeAlerte);

      json += String(ligne) + "\n";
    }

    sqlite3_finalize(stmt);

    if (DEBUGETTEST)
    {
      if (json.length() > 0)
      {
        Serial.print("Alertes envoyées : ");
        Serial.println(json);
      }
    }
  }

  return json;
}

// JUSTE POUR LE TEST UNITAIRE
static int callbackAfficherTestUnitaire(void *data, int nbColonnes, char **valeurs, char **nomColonnes)
{
  for (int i = 0; i < nbColonnes; i++)
  {
    Serial.printf("%s = %s\n", nomColonnes[i], valeurs[i] ? valeurs[i] : "NULL");
  }
  Serial.println("---");
  return 0;
}

// JUSTE POUR LE TEST UNITAIRE
void MemoireProgramme::afficherCalendrierTestUnitaire()
{
  char *errMsg = nullptr;

  if (sqlite3_exec(db, "SELECT * FROM SESSIONS;", callbackAfficherTestUnitaire, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur affichage sessions : %s\n", errMsg);
    sqlite3_free(errMsg);
  }
}

// JUSTE POUR LE TEST UNITAIRE
void MemoireProgramme::afficherAlerteTestUnitaire()
{
  char *errMsg = nullptr;

  if (sqlite3_exec(db, "SELECT * FROM ALERTES;", callbackAfficherTestUnitaire, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur affichage alertes : %s\n", errMsg);
    sqlite3_free(errMsg);
  }
}