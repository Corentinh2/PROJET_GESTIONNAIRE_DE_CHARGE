#include "CommunicationMobile.h"
#include <Arduino.h>

CommunicationMobile::CommunicationMobile(MemoireProgramme *_memoire)
{
    memoire = _memoire;
    clientConnecte = false;
    relais = -1;
    serveur.listen(PORT);
    Serial.println("Serveur WebSocket démarré sur le port 5555 !");
}

CommunicationMobile::~CommunicationMobile()
{
}

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
        client.send(memoire->obtenirTrameOriginale());
    }
    if (action == "supprimerCalendrier")
    {
        memoire->supprimerCalendrier(doc["id"]);
        client.send(memoire->obtenirTrameOriginale());
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