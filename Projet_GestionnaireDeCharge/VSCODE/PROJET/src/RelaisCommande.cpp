#include "RelaisCommande.h"
#include <Arduino.h>

/**
 * @brief Constructeur - configure la broche du relais en sortie
 */
RelaisCommande::RelaisCommande()
{
    pinMode(D1, OUTPUT);
}

/**
 * @brief Ouvre le relais pour arrêter la charge
 *        Met la broche à LOW
 */
void RelaisCommande::ouvrir()
{
    digitalWrite(D1, LOW);
}

/**
 * @brief Ferme le relais pour démarrer la charge
 *        Met la broche à HIGH
 */
void RelaisCommande::fermer()
{
    digitalWrite(D1, HIGH);
}