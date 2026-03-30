#include "GestionnaireCharge.h"
#include <Arduino.h>

GestionnaireCharge::GestionnaireCharge()
{

  horloge = new HorlogeTempsReel;
  if (horloge->obtenirEtat())
  {
    Serial.println("HorlogeTempsReel : OK");
  }
  else
  {
    Serial.println("HorlogeTempsReel : PASOK");
  }
  memoire = new MemoireProgramme;
  if (memoire->obtenirEtat())
  {
    Serial.println("memoire : OK");
  }
  else
  {
    Serial.println("memoire : PASOK");
  }

  if (memoire->obtenirEtat() && horloge->obtenirEtat())
    etat = true;
}

GestionnaireCharge::~GestionnaireCharge()
{
  delete memoire;
  delete horloge;
}

MemoireProgramme *GestionnaireCharge::obtenirMemoire()
{
  return memoire;
}

HorlogeTempsReel *GestionnaireCharge::obtenirHorloge()
{
  return horloge;
}

void GestionnaireCharge::afficherMenuTestUnitaire()
{
  String trame = "";
  String choix = "";
  bool choixValide = false;

  Serial.println("=== MENU ===");
  Serial.println("1 - ajouterCalendrier");
  Serial.println("2 - supprimerCalendrier");
  Serial.println("3 - afficherCalendrierTestUnitaire");
  Serial.println("Votre choix : ");

  while (!choixValide)
  {
    if (Serial.available() > 0)
    {
      char carLu = Serial.read();
      Serial.print(carLu);

      if (carLu == '\n' || carLu == '\r')
      {
        choix.trim();
        if (choix == "1" || choix == "2" || choix == "3")
        {
          choixValide = true;
        }
        else
        {
          Serial.println("\nChoix invalide ! Entrez 1, 2 ou 3:");
          choix = "";
        }
      }
      else
      {
        choix += carLu;
      }
    }
  }

  switch (choix.toInt())
  {
  case 1:
  {
    String trame = "";
    int jours = -1;
    int hd = -1;
    int md = -1;
    int hf = -1;
    int mf = -1;
    bool valide = false;

    Serial.println("\nSaisir trame calendrier (format: jours,hd,md,hf,mf) :");
    Serial.println("jours: 0-127 | hd: 0-23 | md: 0 ou 30 | hf: 0-23 | mf: 0 ou 30");

    while (!valide)
    {
      if (Serial.available() > 0)
      {
        char carLu = Serial.read();
        Serial.print(carLu);

        if (carLu == '\n' || carLu == '\r')
        {
          trame.trim();

          if (trame.length() > 0)
          {
            int virgule1 = trame.indexOf(',');
            int virgule2 = trame.indexOf(',', virgule1 + 1);
            int virgule3 = trame.indexOf(',', virgule2 + 1);
            int virgule4 = trame.indexOf(',', virgule3 + 1);

            if (virgule1 != -1 && virgule2 != -1 && virgule3 != -1 && virgule4 != -1)
            {
              jours = trame.substring(0, virgule1).toInt();
              hd = trame.substring(virgule1 + 1, virgule2).toInt();
              md = trame.substring(virgule2 + 1, virgule3).toInt();
              hf = trame.substring(virgule3 + 1, virgule4).toInt();
              mf = trame.substring(virgule4 + 1).toInt();

              bool joursOk = (jours >= 0 && jours <= 127);
              bool hdOk = (hd >= 0 && hd <= 23);
              bool mdOk = (md == 0 || md == 30);
              bool hfOk = (hf >= 0 && hf <= 23);
              bool mfOk = (mf == 0 || mf == 30);

              if (joursOk && hdOk && mdOk && hfOk && mfOk)
              {
                valide = true;
              }
              else
              {
                Serial.println("\nValeurs invalides ! Reessayez :");
                if (!joursOk)
                  Serial.println("  jours doit etre entre 0 et 127");
                if (!hdOk)
                  Serial.println("  hd doit etre entre 0 et 23");
                if (!mdOk)
                  Serial.println("  md doit etre 0 ou 30");
                if (!hfOk)
                  Serial.println("  hf doit etre entre 0 et 23");
                if (!mfOk)
                  Serial.println("  mf doit etre 0 ou 30");
                trame = "";
              }
            }
            else
            {
              Serial.println("\nFormat invalide ! Exemple : 5,8,30,17,0");
              trame = "";
            }
          }
        }
        else
        {
          trame += carLu;
        }
      }
    }
    obtenirMemoire()->ajouterCalendrier(jours, hd, md, hf, mf);
    Serial.printf("\nCalendrier ajouté : %d,%d,%d,%d,%d\n", jours, hd, md, hf, mf);
    break;
  }
  case 2:
  {
    obtenirMemoire()->afficherCalendrierTestUnitaire();
    Serial.println("\nSaisir l'id du calendrier à supprimer : ");

    String saisie = "";
    bool saisi = false;

    while (!saisi)
    {
      if (Serial.available() > 0)
      {
        char carLu = Serial.read();
        Serial.print(carLu);

        if (carLu == '\n' || carLu == '\r')
        {
          saisie.trim();
          if (saisie.length() > 0)
          {
            saisi = true;
          }
        }
        else
        {
          saisie += carLu;
        }
      }
    }
    obtenirMemoire()->supprimerCalendrier(saisie.toInt());
    Serial.println("Calendrier(s) restant(s) : ");
    obtenirMemoire()->afficherCalendrierTestUnitaire();
    break;
  }
  break;
  case 3:
    obtenirMemoire()->afficherCalendrierTestUnitaire();
    break;
  default:
    break;
  }
}

bool GestionnaireCharge::obtenirEtat() const
{
  return etat;
}

void GestionnaireCharge::syncroniserHorloge()
{
  horloge->syncFromNTP();
}
