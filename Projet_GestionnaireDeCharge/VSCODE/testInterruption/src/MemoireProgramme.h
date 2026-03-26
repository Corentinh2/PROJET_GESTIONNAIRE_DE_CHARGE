#ifndef MEMOIRE_PROGRAMME_H
#define MEMOIRE_PROGRAMME_H

#include <Arduino.h>
#define CHEMIN_BDD "/littlefs/programme.db"

typedef struct sqlite3 sqlite3;

class MemoireProgramme {
  private:
    sqlite3* db;
    bool estOuverte;

    bool creerTables();

  public:
    MemoireProgramme();
    ~MemoireProgramme();
    bool obtenirEtat() const;
    bool ajouterCalendrier(int jours, int hd, int md, int hf, int mf);
    bool lireCalendrier();
    bool nettoyerTables();
    bool decoderJoursTrame(int joursRecu, int* joursDecodes);
};

#endif