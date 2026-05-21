#include <Arduino.h>
#include <WiFi.h>
#include "GestionnaireCharge.h"

#define BP1 39

const char *ssid = "fortinet";
const char *motDePasse = "Projet2026";

GestionnaireCharge *gestionnaire;

void afficherMenuTest()
{
  String trame = "";
  String choix = "";
  bool choixValide = false;

  Serial.println("=== MENU ===");
  Serial.println("1 - ajouterEvenement");
  Serial.println("2 - supprimerCalendrier");
  Serial.println("3 - ajouterAlerte");
  Serial.println("4 - supprimerAlerte");
  Serial.println("5 - obtenirTrameOriginal");
  Serial.println("6 - obtenirAlerte");
  Serial.println("7 - affichercalendrier et alerte");

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
        if (choix == "1" || choix == "2" || choix == "3" || choix == "4" || choix == "5" || choix == "6" || choix == "7")
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
    break;
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
              bool mdOk = true;
              bool hfOk = (hf >= 0 && hf <= 23);
              bool mfOk = true;

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
    gestionnaire->obtenirMemoire()->ajouterEvenement(jours, hd, md, hf, mf);
    Serial.printf("\nCalendrier ajouté : %d,%d,%d,%d,%d\n", jours, hd, md, hf, mf);
    break;
  }
  case 2:
  {
    gestionnaire->obtenirMemoire()->afficherCalendrierTestUnitaire();
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
    gestionnaire->obtenirMemoire()->supprimerCalendrier(saisie.toInt());
    Serial.println("Calendrier(s) restant(s) : ");
    gestionnaire->obtenirMemoire()->afficherCalendrierTestUnitaire();
    break;
  }
  break;
  case 3:
  {
    Serial.println("\nSaisir le type d'alerte (1 = alerte température, 0 = alerte courant): ");

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
    if (saisie.toInt() == 1)
    {
      gestionnaire->obtenirMemoire()->ajouterAlerte(true);
      gestionnaire->obtenirMemoire()->afficherAlerteTestUnitaire();
    }
    else
    {
      gestionnaire->obtenirMemoire()->ajouterAlerte(false);
      gestionnaire->obtenirMemoire()->afficherAlerteTestUnitaire();
    }
  }
  break;
  case 4:
  {
    gestionnaire->obtenirMemoire()->supprimerAlerte();
    gestionnaire->obtenirMemoire()->afficherAlerteTestUnitaire();
  }
  break;
  case 5:
  {
    String trameOriginale = gestionnaire->obtenirMemoire()->obtenirTrameOriginale();
  }
  break;
  case 6:
  {
    String alerte = gestionnaire->obtenirMemoire()->obtenirAlerte();
  }
  break;
  case 7:
  {
    gestionnaire->obtenirMemoire()->afficherCalendrierTestUnitaire();
    gestionnaire->obtenirMemoire()->afficherAlerteTestUnitaire();
  }
  break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  if (DEBUGETTEST)
  {
    delay(2000);
    Serial.println("Démarrage...");
  }

  Wire.begin(); 

  WiFi.begin(ssid, motDePasse);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if (DEBUGETTEST) Serial.print(".");
  }

  if (DEBUGETTEST)
  {
    Serial.println("\nWiFi connecté !");
    delay(1000);
  }

  gestionnaire = new GestionnaireCharge(ssid, motDePasse);

  if (DEBUGETTEST) pinMode(BP1, INPUT);
}

void loop()
{
  if (DEBUGETTEST)
  {
    if (digitalRead(BP1) == LOW)
    {
      afficherMenuTest();
      delay(1000);
    }
  }
  gestionnaire->controler();
}
