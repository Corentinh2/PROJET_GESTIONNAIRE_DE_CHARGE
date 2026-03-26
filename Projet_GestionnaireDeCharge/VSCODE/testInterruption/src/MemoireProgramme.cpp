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
        Serial.println("MemoireProgramme : OK");
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
      "id_calendrier INTEGER PRIMARY KEY,"
      "jours INTEGER,"
      "hd INTEGER,"
      "md INTEGER,"
      "hf INTEGER,"
      "mf INTEGER"
      ");";

  const char *requeteAlerte =
      "CREATE TABLE IF NOT EXISTS ALERTE ("
      "id_alerte INTEGER PRIMARY KEY,"
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

bool MemoireProgramme::ajouterCalendrier(int joursRecu, int hd, int md, int hf, int mf) {
  bool succes = true;
  int joursDecodes[8];
  char requete[200];
  char* errMsg = nullptr;

  succes = decoderJoursTrame(joursRecu, joursDecodes);

  for (int i = 0; joursDecodes[i] != -1 && succes; i++) {
    snprintf(requete, sizeof(requete),
      "INSERT INTO CALENDRIER (jours, hd, md, hf, mf) VALUES (%d, %d, %d, %d, %d);",
      joursDecodes[i], hd, md, hf, mf
    );

    if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK) {
      Serial.printf("Erreur ajout calendrier : %s\n", errMsg);
      sqlite3_free(errMsg);
      succes = false;
    }
  }

  if (succes) {
    Serial.println("Calendrier ajouté !");
  }

  return succes;
}

static int callbackCalendrier(void *data, int nbColonnes, char **valeurs, char **nomColonnes)
{
  for (int i = 0; i < nbColonnes; i++)
  {
    Serial.printf("%s = %s\n", nomColonnes[i], valeurs[i] ? valeurs[i] : "NULL");
  }
  Serial.println("---");
  return 0;
}

bool MemoireProgramme::lireCalendrier()
{
  bool succes = true;
  char *errMsg = nullptr;

  const char *requete = "SELECT * FROM CALENDRIER;";

  if (sqlite3_exec(db, requete, callbackCalendrier, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur lecture calendrier : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes)
  {
    Serial.println("Lecture calendrier terminée !");
  }

  return succes;
}

bool MemoireProgramme::nettoyerTables()
{
  bool succes = true;
  char *errMsg = nullptr;

  if (sqlite3_exec(db, "DELETE FROM CALENDRIER;", nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur nettoyage CALENDRIER : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes && sqlite3_exec(db, "DELETE FROM ALERTE;", nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur nettoyage ALERTE : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes)
  {
    Serial.println("Tables nettoyées !");
  }

  return succes;
}

bool MemoireProgramme::decoderJoursTrame(int joursRecu, int* joursDecodes) {
  bool succes = true;
  int index = 0;

  for (int i = 0; i < 7; i++) {
    if (joursRecu & (1 << i)) {
      joursDecodes[index] = i + 1;  // lundi=1, mardi=2 etc.
      index++;
    }
  }

  joursDecodes[index] = -1;  // Marqueur de fin

  if (index == 0) {
    Serial.println("Aucun jour valide recu !");
    succes = false;
  }

  return succes;
}
