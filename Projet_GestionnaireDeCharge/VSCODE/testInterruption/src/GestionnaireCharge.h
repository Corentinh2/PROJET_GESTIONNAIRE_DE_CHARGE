#ifndef GESTIONNAIRE_CHARGE_H
#define GESTIONNAIRE_CHARGE_H

#include "HorlogeTempsReel.h"
#include "MemoireProgramme.h"

class GestionnaireCharge
{

private:
  HorlogeTempsReel *horloge;
  MemoireProgramme *memoire;
  bool etat;

public:
  GestionnaireCharge();
  ~GestionnaireCharge();
  MemoireProgramme* obtenirMemoire();
  HorlogeTempsReel* obtenirHorloge();
  void afficherMenu();
  bool obtenirEtat() const;
  void syncroniserHorloge();
};

#endif