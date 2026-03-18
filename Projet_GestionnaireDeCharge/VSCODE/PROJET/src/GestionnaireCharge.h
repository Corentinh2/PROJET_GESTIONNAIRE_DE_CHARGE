#ifndef GESTIONNAIRE_CHARGE_H
#define GESTIONNAIRE_CHARGE_H

#include "HorlogeTempsReel.h"
#include "MemoireProgramme.h"

class GestionnaireCharge {

  public:
    HorlogeTempsReel horloge;
    MemoireProgramme memoire;

    bool begin();

};

#endif