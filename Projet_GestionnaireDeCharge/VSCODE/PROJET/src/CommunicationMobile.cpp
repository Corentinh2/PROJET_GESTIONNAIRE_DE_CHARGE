#include "CommunicationMobile.h"
#include <Arduino.h>

CommunicationMobile::CommunicationMobile(MemoireProgramme *_memoire, RelaisCommande *_relais)
{
    memoire = _memoire;
    relais = _relais;
    clientConnecte = false;

    serveur.listen(81);
    Serial.println("Serveur WebSocket démarré sur le port 81 !");
}

CommunicationMobile::~CommunicationMobile()
{
}

void CommunicationMobile::traiterMessage(String message)
{
    StaticJsonDocument<1024> doc;
    DeserializationError erreur = deserializeJson(doc, message);

    String json = memoire->obtenirTrameOriginale();

    if (erreur)
    {
        Serial.printf("Erreur JSON : %s\n", erreur.c_str());
        return;
    }

    String action = doc["action"].as<String>();

    if (action == "obtenirCalendrier")
    {
        client.send(json);
    }

    if (action == "ajouterCalendrier")
    {
        int jours = doc["jours"];
        int hd = doc["hd"];
        int md = doc["md"];
        int hf = doc["hf"];
        int mf = doc["mf"];
        memoire->ajouterEvenement(jours, hd, md, hf, mf);
        client.send(json);
    }

    if (action == "supprimerCalendrier")
    {
        int id = doc["id"];
        memoire->supprimerCalendrier(id);
        client.send(json);
    }

    if (action == "marcheForcee")
    {
        bool activer = doc["activer"];
        if (activer)
        {
            Serial.println("Marche forcée activée !");
            relais->fermer();
        }
        if (!activer)
        {
            Serial.println("Marche forcée désactivée !");
            relais->ouvrir();
        }
    }
}

void CommunicationMobile::gerer()
{
    if (serveur.poll())
    {
        client = serveur.accept();
        clientConnecte = true;
        Serial.println("Client connecté !");
    }

    if (clientConnecte && client.available())
    {
        WebsocketsMessage msg = client.readBlocking();
        Serial.printf("Message reçu : %s\n", msg.data().c_str());
        traiterMessage(msg.data());
    }
}