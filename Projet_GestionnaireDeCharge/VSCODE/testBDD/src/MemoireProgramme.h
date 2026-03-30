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
    int  obtenirProchainIdCalendrier();

  public:
    MemoireProgramme();
    ~MemoireProgramme();
    bool obtenirEtat() const;
    bool ajouterCalendrier(int _jours, int _hd, int _md, int _hf, int _mf);
    bool lireCalendrier(int _jour, int _heure, int _minute, bool _debut);
    bool supprimerCalendrier(int _id); 
    bool decoderJoursTrame(int _joursRecu, int* _joursDecodes);
    void afficherCalendrierTestUnitaire();
};

#endif