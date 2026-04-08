#include "RelaisCommande.h"
#include <Arduino.h>

RelaisCommande::RelaisCommande()
{
    pinMode(D1, OUTPUT);
}

RelaisCommande::~RelaisCommande()
{
}

void RelaisCommande::ouvrir()
{
    digitalWrite(D1, LOW);
}

void RelaisCommande::fermer()
{
    digitalWrite(D1, HIGH);
}
