#include "RelaisCommande.h"

/**
 * @brief Constructeur - configure la broche du relais en sortie
 */
RelaisCommande::RelaisCommande()
{
    pinMode(PIN_RELAIS, OUTPUT);
}

/**
 * @brief Ouvre le relais pour arrêter la charge
 *        Met la broche à LOW
 */
void RelaisCommande::ouvrir()
{
    digitalWrite(PIN_RELAIS, LOW);
}

/**
 * @brief Ferme le relais pour démarrer la charge
 *        Met la broche à HIGH
 */
void RelaisCommande::fermer()
{
    digitalWrite(PIN_RELAIS, HIGH);
}