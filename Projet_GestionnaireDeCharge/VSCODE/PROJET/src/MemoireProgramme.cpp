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

bool MemoireProgramme::ajouterEvenement(int _joursRecu, int _hd, int _md, int _hf, int _mf)
{
  bool succes = true;
  int joursDecodes[8];
  bool aCheval = (_hf < _hd);

  succes = decoderJoursTrame(_joursRecu, joursDecodes);

  if (succes)
  {
    int idCalendrier = obtenirProchainIdCalendrier();

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
        succes = insererCalendrier(idCalendrier, jourActuel, _hd, _md, 23, 59);

        if (succes)
        {
          succes = insererCalendrier(idCalendrier, jourSuivant, 0, 0, _hf, _mf);
        }
      }

      if (!aCheval)
      {
        succes = insererCalendrier(idCalendrier, jourActuel, _hd, _md, _hf, _mf);
      }
    }
  }

  if (succes)
  {
    Serial.printf("Calendrier %d ajouté avec succès !\n", obtenirProchainIdCalendrier() - 1);
  }

  return succes;
}

bool MemoireProgramme::insererCalendrier(int id, int jour, int hd, int md, int hf, int mf)
{
  bool succes = true;
  char *errMsg = nullptr;
  char requete[256];

  snprintf(requete, sizeof(requete),
           "INSERT INTO CALENDRIER (id_calendrier, jours, hd, md, hf, mf) "
           "VALUES (%d, %d, %d, %d, %d, %d);",
           id, jour, hd, md, hf, mf);

  if (sqlite3_exec(db, requete, nullptr, nullptr, &errMsg) != SQLITE_OK)
  {
    Serial.printf("Erreur insertion : %s\n", errMsg);
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

String MemoireProgramme::obtenirTrameOriginale()
{
  String json = "";
  sqlite3_stmt *stmt = nullptr;

  const char *requete =
      "SELECT id_calendrier, jours, hd, md, hf, mf "
      "FROM CALENDRIER "
      "ORDER BY id_calendrier, jours;";

  if (sqlite3_prepare_v2(db, requete, -1, &stmt, nullptr) != SQLITE_OK)
  {
    Serial.printf("Erreur préparation : %s\n", sqlite3_errmsg(db));
    return json;
  }

  int idCourant = -1;
  int masqueJoursTous = 0;
  int masqueJoursStart = 0;
  int hd = 0, md = 0, hf = 0, mf = 0;
  bool premiereLigne = true;

  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    int idCalendrier = sqlite3_column_int(stmt, 0);
    int jour = sqlite3_column_int(stmt, 1);
    int hdLigne = sqlite3_column_int(stmt, 2);
    int mdLigne = sqlite3_column_int(stmt, 3);
    int hfLigne = sqlite3_column_int(stmt, 4);
    int mfLigne = sqlite3_column_int(stmt, 5);

    if (idCalendrier != idCourant)
    {
      if (idCourant != -1)
      {
        // Si hd > hf, c'est un créneau de nuit : on ne garde que les jours de départ (masqueJoursStart)
        // Sinon, on garde tous les jours (masqueJoursTous)
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
      idCourant = idCalendrier;
      masqueJoursTous = 0;
      masqueJoursStart = 0;
      premiereLigne = true;
    }

    // On accumule tous les jours vus
    masqueJoursTous = masqueJoursTous | (1 << (jour - 1));

    // On isole les jours de "départ" (qui ne sont pas des restes de nuit commençant à 00h00)
    if (hdLigne != 0 || mdLigne != 0)
    {
      masqueJoursStart |= (1 << (jour - 1));
    }

    // Initialisation avec la première ligne trouvée
    if (premiereLigne)
    {
      hd = hdLigne;
      md = mdLigne;
      hf = hfLigne;
      mf = mfLigne;
      premiereLigne = false;
    }

    // Si on trouve la partie "début" d'un créneau à cheval, on récupère hd/md
    if (hfLigne == 23 && mfLigne == 59)
    {
      hd = hdLigne;
      md = mdLigne;
    }

    // Si on trouve la partie "fin" d'un créneau à cheval, on récupère hf/mf
    if (hdLigne == 0 && mdLigne == 0)
    {
      hf = hfLigne;
      mf = mfLigne;
    }
  }

  // Ajout du tout dernier calendrier traité
  if (idCourant != -1)
  {
    int joursFinal = (hd > hf) ? masqueJoursStart : masqueJoursTous;
    char ligne[256];
    snprintf(ligne, sizeof(ligne),
             "{\"action\":\"calendrier\",\"id\":%d,\"jours\":%d,\"hd\":%d,\"md\":%d,\"hf\":%d,\"mf\":%d}",
             idCourant, joursFinal, hd, md, hf, mf);
    json += String(ligne) + "\n";
  }

  sqlite3_finalize(stmt);
  Serial.println(json);
  return json;
}