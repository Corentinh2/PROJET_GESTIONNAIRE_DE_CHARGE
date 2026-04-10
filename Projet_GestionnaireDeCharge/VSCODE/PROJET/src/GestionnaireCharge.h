#ifndef GESTIONNAIRE_CHARGE_H
#define GESTIONNAIRE_CHARGE_H

#include "HorlogeTempsReel.h"
#include "MemoireProgramme.h"
#include "RelaisCommande.h"
#include "CommunicationMobile.h"
#include "CapteurTemp.h"

#define TEMPMAX 50
class GestionnaireCharge
{

private:
  HorlogeTempsReel *horloge;
  MemoireProgramme *memoire;
  RelaisCommande *relais;
  CommunicationMobile *communication;
  CapteurTemp *ds18s20;

  bool etat;
  bool chargeEnCourt;
  bool marcheForceeActive;

public:
  GestionnaireCharge();
  ~GestionnaireCharge();
  bool obtenirEtat() const;
  void syncroniserHorloge();


  void controler();

  MemoireProgramme *obtenirMemoire();
  HorlogeTempsReel *obtenirHorloge();
};

#endif