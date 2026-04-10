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
    bool ajouterEvenement(int _jours, int _hd, int _md, int _hf, int _mf);
    bool insererCalendrier(int id, int jour, int hd, int md, int hf, int mf);
    bool lireCalendrier(int _jour, int _heure, int _minute, bool _debut);
    bool supprimerCalendrier(int _id); 
    bool decoderJoursTrame(int _joursRecu, int* _joursDecodes);
    bool ajouterAlerte(bool type);
    bool supprimerAlerte();

    String obtenirTrameOriginale();
    String obtenirAlerte();


    void afficherCalendrierTestUnitaire();

};

#endif