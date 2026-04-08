#ifndef GESTIONNAIRE_CHARGE_H
#define GESTIONNAIRE_CHARGE_H

#include "HorlogeTempsReel.h"
#include "MemoireProgramme.h"
#include "RelaisCommande.h"
#include "CommunicationMobile.h"

class GestionnaireCharge
{

private:
  HorlogeTempsReel *horloge;
  MemoireProgramme *memoire;
  RelaisCommande *relais;
  CommunicationMobile *communication;

  bool etat;

public:
  GestionnaireCharge();
  ~GestionnaireCharge();
  bool obtenirEtat() const;
  void syncroniserHorloge();
  void interrogerCalendrier();

  void gererCommunication();

  MemoireProgramme *obtenirMemoire();
  HorlogeTempsReel *obtenirHorloge();
};

#endif