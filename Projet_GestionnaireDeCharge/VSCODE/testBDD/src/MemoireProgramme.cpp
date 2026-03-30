#include "MemoireProgramme.h"
#include <LittleFS.h>
#include <sqlite3.h>
#include <Arduino.h>

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
        Serial.println("MemoireProgramme : Erreur création tables !");
      }
    }
    else
    {
      Serial.printf("MemoireProgramme : Erreur BDD : %s\n", sqlite3_errmsg(db));
    }
  }
  else
  {
    Serial.println("MemoireProgramme : Erreur montage LittleFS !");
  }
}

MemoireProgramme::~MemoireProgramme()
{
  if (estOuverte)
  {
    sqlite3_close(db);
    estOuverte = false;
    Serial.println("MemoireProgramme : fermée.");
  }
}

bool MemoireProgramme::obtenirEtat() const
{
  return estOuverte;
}

bool MemoireProgramme::creerTables()
{
  bool succes = true;
  char *errMsg = nullptr;

  const char *requeteCalendrier =
      "CREATE TABLE IF NOT EXISTS CALENDRIER ("
      "id_ligne      INTEGER PRIMARY KEY AUTOINCREMENT,"
      "id_calendrier INTEGER,"
      "jours         INTEGER,"
      "hd            INTEGER,"
      "md            INTEGER,"
      "hf            INTEGER,"
      "mf            INTEGER"
      ");";

  const char *requeteAlerte =
      "CREATE TABLE IF NOT EXISTS ALERTE ("
      "id_alerte   INTEGER PRIMARY KEY,"
      "type_alerte BOOLEAN"
      ");";

  if (sqlite3_exec(db, requeteCalendrier, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur table CALENDRIER : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes && sqlite3_exec(db, requeteAlerte, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur table ALERTE : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  return succes;
}

int MemoireProgramme::obtenirProchainIdCalendrier()
{
  sqlite3_stmt *stmt = nullptr;
  int prochain = 1;

  const char *requete = "SELECT COALESCE(MAX(id_calendrier), 0) + 1 FROM CALENDRIER;";

  if (sqlite3_prepare_v2(db, requete, -1, &stmt, nullptr) == SQLITE_OK)
  {
    if (sqlite3_step(stmt) == SQLITE_ROW)
      prochain = sqlite3_column_int(stmt, 0);
  }

  sqlite3_finalize(stmt);
  return prochain;
}

bool MemoireProgramme::ajouterCalendrier(int _joursRecu, int _hd, int _md, int _hf, int _mf)
{
  bool succes = true;
  int joursDecodes[8];
  char requete[256];
  char *errMsg = nullptr;
  bool aCheval = (_hf < _hd);

  succes = decoderJoursTrame(_joursRecu, joursDecodes);

  if (succes)
  {
    int idCalendrier = obtenirProchainIdCalendrier();

    for (int i = 0; joursDecodes[i] != -1 && succes; i++)
    {
      int jourActuel = joursDecodes[i];
      int jourSuivant = (jourActuel == 7) ? 1 : jourActuel + 1;

      if (aCheval)
      {
        snprintf(requete, sizeof(requete),
                 "INSERT INTO CALENDRIER (id_calendrier, jours, hd, md, hf, mf) "
                 "VALUES (%d, %d, %d, %d, 0, 0);",
                 idCalendrier, jourActuel, _hd, _md);

        if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
        {
          Serial.printf("Erreur ajout jour actuel : %s\n", errMsg);
          sqlite3_free(errMsg);
          succes = false;
        }

        if (succes)
        {
          snprintf(requete, sizeof(requete),
                   "INSERT INTO CALENDRIER (id_calendrier, jours, hd, md, hf, mf) "
                   "VALUES (%d, %d, 0, 0, %d, %d);",
                   idCalendrier, jourSuivant, _hf, _mf);

          if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
          {
            Serial.printf("Erreur ajout jour suivant : %s\n", errMsg);
            sqlite3_free(errMsg);
            succes = false;
          }
        }
      }
      else
      {
        snprintf(requete, sizeof(requete),
                 "INSERT INTO CALENDRIER (id_calendrier, jours, hd, md, hf, mf) "
                 "VALUES (%d, %d, %d, %d, %d, %d);",
                 idCalendrier, jourActuel, _hd, _md, _hf, _mf);

        if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
        {
          Serial.printf("Erreur ajout calendrier : %s\n", errMsg);
          sqlite3_free(errMsg);
          succes = false;
        }
      }
    }
  }

  if (succes)
    Serial.printf("Calendrier %d ajouté avec succès !\n", obtenirProchainIdCalendrier() - 1);

  return succes;
}

static int callbackCalendrier(void *data, int nbColonnes, char **valeurs, char **nomColonnes)
{
  bool *trouve = (bool *)data;
  *trouve = true;
  return 0;
}

bool MemoireProgramme::lireCalendrier(int _jour, int _heure, int _minute, bool _debut)
{
  bool succes = true;
  bool trouve = false;
  char *errMsg = nullptr;
  char requete[256];

  if (_debut)
  {
    snprintf(requete, sizeof(requete),
             "SELECT * FROM CALENDRIER WHERE jours = %d AND hd = %d AND md = %d;",
             _jour, _heure, _minute);
  }
  else
  {
    snprintf(requete, sizeof(requete),
             "SELECT * FROM CALENDRIER WHERE jours = %d AND hf = %d AND mf = %d;",
             _jour, _heure, _minute);
  }

  if (sqlite3_exec(db, requete, callbackCalendrier, &trouve, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur lecture calendrier : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes)
    succes = trouve;

  return succes;
}

bool MemoireProgramme::supprimerCalendrier(int _id)
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[128];

  snprintf(requete, sizeof(requete),
           "DELETE FROM CALENDRIER WHERE id_calendrier = %d;", _id);

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur suppression CALENDRIER : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes)
    Serial.printf("Calendrier %d supprimé !\n", _id);

  return succes;
}

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

  _joursDecodes[index] = -1; // Marqueur de fin

  if (index == 0)
  {
    Serial.println("Aucun jour valide recu !");
    succes = false;
  }

  return succes;
}

static int callbackAfficherTestUnitaire(void *data, int nbColonnes, char **valeurs, char **nomColonnes)
{
  for (int i = 0; i < nbColonnes; i++)
  {
    Serial.printf("%s = %s\n", nomColonnes[i], valeurs[i] ? valeurs[i] : "NULL");
  }
  Serial.println("---");
  return 0;
}

void MemoireProgramme::afficherCalendrierTestUnitaire()
{
  char *errMsg = nullptr;

  if (sqlite3_exec(db, "SELECT * FROM CALENDRIER;", callbackAfficherTestUnitaire, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur affichage calendrier : %s\n", errMsg);
    sqlite3_free(errMsg);
  }
}