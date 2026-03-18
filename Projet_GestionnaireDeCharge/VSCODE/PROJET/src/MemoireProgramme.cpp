#include "MemoireProgramme.h"
#include <LittleFS.h>  
#include <sqlite3.h>

MemoireProgramme::MemoireProgramme() {
  _db = nullptr;
  _estOuverte = false;
}

bool MemoireProgramme::begin() {
  bool succes = true;

  Serial.println("MemoireProgramme::begin() démarré");

  if (!LittleFS.begin(true)) {
    Serial.println("Erreur montage LittleFS !");
    succes = false;
  }

  Serial.println("LittleFS monté !");

  if (succes && sqlite3_open("/littlefs/programme.db", &_db) != SQLITE_OK) {
    Serial.printf("Erreur ouverture BDD : %s\n", sqlite3_errmsg(_db));
    succes = false;
  }

  Serial.println("BDD ouverte !");

  if (succes && !creerTables()) {
    Serial.println("Erreur création des tables !");
    succes = false;
  }

  if (succes) {
    Serial.println("Base de données prête !");
    _estOuverte = true;
  }

  return succes;
}

bool MemoireProgramme::creerTables() {
  bool succes = false;
  char* errMsg = nullptr;

  const char* requeteCalendrier =
    "CREATE TABLE IF NOT EXISTS CALENDRIER ("
    "id_calendrier INTEGER PRIMARY KEY,"
    "jours INTEGER,"
    "hd INTEGER,"
    "md INTEGER,"
    "hf INTEGER,"
    "mf INTEGER"
    ");";

  const char* requeteAlerte =
    "CREATE TABLE IF NOT EXISTS ALERTE ("
    "id_alerte INTEGER PRIMARY KEY,"
    "type_alerte BOOLEAN"
    ");";

  if (sqlite3_exec(_db, requeteCalendrier, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    Serial.printf("Erreur table CALENDRIER : %s\n", errMsg);
    sqlite3_free(errMsg);
  } else if (sqlite3_exec(_db, requeteAlerte, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    Serial.printf("Erreur table ALERTE : %s\n", errMsg);
    sqlite3_free(errMsg);
  } else {
    Serial.println("Tables créées !");
    succes = true;
  }

  return succes;
}

void MemoireProgramme::fermer() {
  if (_estOuverte) {
    sqlite3_close(_db);
    _estOuverte = false;
    Serial.println("Base de données fermée.");
  }
}

bool MemoireProgramme::ajouterCalendrier() {
  bool succes = true;
  char* errMsg = nullptr;
  char requete[200];

  snprintf(requete, sizeof(requete),
    "INSERT INTO CALENDRIER (jours, hd, md, hf, mf) VALUES (2, 8, 30, 10, 0);"
  );

  if (sqlite3_exec(_db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK) {
    Serial.printf("Erreur ajout calendrier : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes) {
    Serial.println("Calendrier ajouté !");
  }

  return succes;
}

static int callbackCalendrier(void* data, int nbColonnes, char** valeurs, char** nomColonnes) {
  for (int i = 0; i < nbColonnes; i++) {
    Serial.printf("%s = %s\n", nomColonnes[i], valeurs[i] ? valeurs[i] : "NULL");
  }
  Serial.println("---");
  return 0;
}

bool MemoireProgramme::lireCalendrier() {
  bool succes = true;
  char* errMsg = nullptr;

  const char* requete = "SELECT * FROM CALENDRIER;";

  if (sqlite3_exec(_db, requete, callbackCalendrier, nullptr, &errMsg) != SQLITE_OK) {
    Serial.printf("Erreur lecture calendrier : %s\n", errMsg);
    sqlite3_free(errMsg);
    succes = false;
  }

  if (succes) {
    Serial.println("Lecture calendrier terminée !");
  }

  return succes;
}