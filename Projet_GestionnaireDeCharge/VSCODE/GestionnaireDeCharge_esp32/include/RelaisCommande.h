#ifndef RELAIS_COMMANDE_H
#define RELAIS_COMMANDE_H

/** @brief Broche de commande du relais (LED rouge en test) */
#define D1 13

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