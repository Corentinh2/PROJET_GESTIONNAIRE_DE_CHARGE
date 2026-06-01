#ifndef RELAIS_COMMANDE_H
#define RELAIS_COMMANDE_H

#include <Arduino.h>
#include <Wire.h>
#include "constantes.h"

/**
 * @brief Classe de gestion du relais de commande de la charge
 */
class RelaisCommande
{
private:

public:
    /**
     * @brief Constructeur - configure la broche du relais en sortie
     */
    RelaisCommande();

    /**
     * @brief Ouvre le relais pour arrêter la charge
     */
    void ouvrir();

    /**
     * @brief Ferme le relais pour démarrer la charge
     */
    void fermer();
};

#endif