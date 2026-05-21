#ifndef COMMUNICATION_MOBILE_H
#define COMMUNICATION_MOBILE_H

#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include "MemoireProgramme.h"

/** @brief Port d'écoute du serveur WebSocket */
#define PORT 5555

using namespace websockets;

/**
 * @brief Classe de gestion de la communication WebSocket avec l'application mobile
 */
class CommunicationMobile
{
private:
    /** @brief Serveur WebSocket */
    WebsocketsServer serveur;

    /** @brief Client WebSocket connecté */
    WebsocketsClient client;

    /** @brief Pointeur vers la mémoire programme pour accéder aux données */
    MemoireProgramme *memoire;

    /** @brief Indique si un client est actuellement connecté */
    bool clientConnecte;

    /** @brief État de la marche forcée : 1 = activer, 0 = désactiver, -1 = pas de message */
    int relais;

    /**
     * @brief Traite un message JSON reçu du client
     * @param data Message JSON reçu sous forme de String
     */
    void traiterMessage(String data);

public:
    /**
     * @brief Constructeur - initialise le serveur WebSocket et démarre l'écoute
     * @param memoire Pointeur vers la mémoire programme
     */
    CommunicationMobile(MemoireProgramme *memoire);

    /**
     * @brief Destructeur
     */
    ~CommunicationMobile();

    /**
     * @brief Gère les connexions et messages WebSocket entrants
     * @return 1 si marche forcée activée, 0 si désactivée, -1 si aucun message de marche forcée
     */
    int gererCommunication();
};

#endif