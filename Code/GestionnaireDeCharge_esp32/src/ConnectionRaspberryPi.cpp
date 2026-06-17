/**
 * @file ConnectionRaspberryPi.cpp
 * @brief Implémentation de la classe ConnectionRaspberryPi pour la communication
 *        Wi-Fi et WebSocket entre l'ESP32 et le serveur Qt du Raspberry Pi.
 *
 * Utilise la bibliothèque ArduinoWebsockets (gilmaimon).
 */

#include "ConnectionRaspberryPi.h"
#include "constantes.h"

/**
 * @brief Constructeur par défaut.
 */
ConnectionRaspberryPi::ConnectionRaspberryPi()
{
        // Enregistrement du callback de réception de messages
    _webSocket.onMessage([](WebsocketsMessage message) {
        if (DEBUGETTEST) Serial.println("[WS] Message reçu : " + message.data());
    });

    // Enregistrement du callback d'événements (connexion / déconnexion)
    _webSocket.onEvent([](WebsocketsEvent event, String data) {
        switch (event)
        {
        case WebsocketsEvent::ConnectionOpened:
            if (DEBUGETTEST) Serial.println("[WS] Connecté au serveur Qt !");
            break;
        case WebsocketsEvent::ConnectionClosed:
            if (DEBUGETTEST) Serial.println("[WS] Déconnecté du serveur Qt !");
            break;
        case WebsocketsEvent::GotPing:
            if (DEBUGETTEST) Serial.println("[WS] Ping reçu.");
            break;
        default:
            break;
        }
    });

    // Connexion au serveur WebSocket du Raspberry Pi
    String url = "ws://" + String(adresseRaspi) + ":" + String(portRaspi) + "/";
    bool connected = _webSocket.connect(url);

    if (connected)
        if (DEBUGETTEST) Serial.println("[WS] Connexion au serveur Qt établie.");
    else
        if (DEBUGETTEST) Serial.println("[WS] Échec de connexion au serveur Qt.");
}

/**
 * @brief Envoie un message texte au serveur Qt via WebSocket.
 *
 * @param msg  Chaîne de caractères à envoyer.
 */
void ConnectionRaspberryPi::EnvoyerNotification(String msg)
{
    if (_webSocket.available())
    {
        _webSocket.send(msg);
        if (DEBUGETTEST) Serial.println("[WS] Message envoyé : " + msg);
    }
    else
    {
        if (DEBUGETTEST) Serial.println("[WS] Erreur : Non connecté au serveur Qt");
    }
}

/**
 * @brief Clôture la session en notifiant le serveur Qt et en fermant la connexion.
 */
void ConnectionRaspberryPi::cloturerSession()
{
    EnvoyerNotification("STOP;0;0");
    _webSocket.close();
}

/**
 * @brief Maintient la connexion WebSocket active (à appeler dans loop()).
 */
void ConnectionRaspberryPi::maintenirConnexion()
{
    _webSocket.poll();
}

/**
 * @brief Construit et envoie une alerte structurée en JSON.
 *
 * @param type   0 = alerte courant, 1 = alerte température.
 * @param texte  Message descriptif de l'alerte.
 */
void ConnectionRaspberryPi::EnvoyerAlerte(int type, String texte)
{
    StaticJsonDocument<200> doc;
    doc["action"]  = "alerte";
    doc["type"]    = type;
    doc["message"] = texte;
    doc["idBorne"] = 5;

    String trame;
    serializeJson(doc, trame);

    EnvoyerNotification(trame);
}

/**
 * @brief Construit et envoie la puissance moyenne en JSON.
 *
 * @param moyennePuissance  Moyenne de la puissance (W).
 */
void ConnectionRaspberryPi::EnvoyerMesures(float moyennePuissance)
 { 
    StaticJsonDocument<200> doc;
    doc["puissance"] = moyennePuissance; 
    doc["idBorne"] = 5; 
    String trame;
    serializeJson(doc, trame);
    if (DEBUGETTEST) Serial.println("[WS] Envoi trame : " + trame); 
    EnvoyerNotification(trame); 
}