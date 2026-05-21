#include "CommunicationMobile.h"
#include <Arduino.h>

/**
 * @brief Constructeur - initialise la mémoire, démarre le serveur WebSocket sur le port défini
 * @param _memoire Pointeur vers la mémoire programme
 */
CommunicationMobile::CommunicationMobile(MemoireProgramme *_memoire)
{
    memoire = _memoire;
    clientConnecte = false;
    relais = -1;
    serveur.listen(PORT);
    Serial.println("Serveur WebSocket démarré sur le port 5555 !");
}

/**
 * @brief Destructeur
 */
CommunicationMobile::~CommunicationMobile()
{
    delete memoire;
}

/**
 * @brief Traite un message JSON reçu et exécute l'action correspondante
 * @param data Message JSON reçu sous forme de String
 */
void CommunicationMobile::traiterMessage(String data)
{
    JsonDocument doc;
    DeserializationError erreur = deserializeJson(doc, data);
    if (erreur)
    {
        Serial.printf("Erreur JSON : %s\n", erreur.c_str());
        return;
    }

    String action = doc["action"].as<String>();
    if (action == "obtenirCalendrier")
    {
        client.send(memoire->obtenirTrameOriginale());
        client.send(memoire->obtenirAlerte());
        memoire->supprimerAlerte();
    }
    if (action == "ajouterCalendrier")
    {
        memoire->ajouterEvenement(doc["jours"], doc["hd"], doc["md"], doc["hf"], doc["mf"]);
    }
    if (action == "supprimerCalendrier")
    {
        memoire->supprimerCalendrier(doc["id"]);
    }
    if (action == "marcheForcee")
    {
        if (doc["activer"])
        {
            relais = 1;
        }
        else
        {
            relais = 0;
        }
    }
}

/**
 * @brief Gère les connexions entrantes et les messages WebSocket de manière non bloquante
 * @return 1 si marche forcée activée, 0 si désactivée, -1 si aucun message de marche forcée
 */
int CommunicationMobile::gererCommunication()
{
    relais = -1;

    if (serveur.poll())
    {
        WebsocketsClient nouveau = serveur.accept();
        if (nouveau.available())
        {
            client = nouveau;
            clientConnecte = true;
            Serial.println("Client connecté !");

            client.onMessage([this](WebsocketsMessage msg) {
                Serial.printf("Message reçu : %s\n", msg.data().c_str());
                traiterMessage(msg.data());
            });

            client.onEvent([this](WebsocketsEvent event, String data) {
                if (event == WebsocketsEvent::ConnectionClosed)
                {
                    clientConnecte = false;
                    Serial.println("Client déconnecté !");
                }
            });
        }
    }

    if (clientConnecte && client.available())
    {
        client.poll();
    }

    return relais;
}