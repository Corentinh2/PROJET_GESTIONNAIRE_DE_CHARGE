#ifndef MEMOIRE_PROGRAMME_H
#define MEMOIRE_PROGRAMME_H

#include <Arduino.h>

typedef struct sqlite3 sqlite3;

class MemoireProgramme {
  private:
    sqlite3* _db;
    bool _estOuverte;



  public:
    MemoireProgramme();
    bool begin();
    void fermer();
    bool creerTables();
    bool ajouterCalendrier();
    bool lireCalendrier();

};

#endif