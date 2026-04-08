#ifndef RELAIS_COMMANDE_H
#define RELAIS_COMMANDE_H

#define D1 13 // définition de la LED rouge

class RelaisCommande
{
private:
public:
    RelaisCommande();
    ~RelaisCommande();

    void ouvrir();
    void fermer();
};

#endif