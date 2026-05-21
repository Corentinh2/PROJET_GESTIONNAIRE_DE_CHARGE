#include "ConnectionRaspberryPi.h"

ConnectionRaspberryPi::ConnectionRaspberryPi()
{
    // Constructeur vide
}

void ConnectionRaspberryPi::initialiserConnexion()
{

#if 0
    // Connexion Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connexion au Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnecté !");
    Serial.print("Adresse IP de l'ESP32 : ");
    Serial.println(WiFi.localIP());
#endif

    // Callback déclenché à chaque message reçu
    _webSocket.onMessage([](WebsocketsMessage message)
                         { Serial.println("[WS] Message reçu : " + message.data()); });

    // Callback déclenché sur les événements de connexion/déconnexion
    _webSocket.onEvent([this](WebsocketsEvent event, String data)
                       {
        switch (event) {
            case WebsocketsEvent::ConnectionOpened:
                Serial.println("[WS] Connecté au serveur Qt !");
                _connecte = true;
                break;
            case WebsocketsEvent::ConnectionClosed:
                Serial.println("[WS] Déconnecté du serveur Qt !");
                _connecte = false;
                break;
            case WebsocketsEvent::GotPing:
                Serial.println("[WS] Ping reçu");
                break;
            default:
                break;
        } });

    // Construction de l'URL WebSocket et tentative de connexion
    String url = "ws://" + String(adresseRaspi) + ":" + String(portRaspi) + "/";
    Serial.println("[WS] Tentative de connexion au serveur Qt...");
    _connecte = _webSocket.connect(url);

    if (_connecte)
    {
        Serial.println("[WS] Connexion WebSocket établie.");
    }
    else
    {
        Serial.println("[WS] Échec de la connexion WebSocket.");
    }
}

void ConnectionRaspberryPi::EnvoyerNotification(String msg)
{
    if (_connecte && _webSocket.available())
    {
        _webSocket.send(msg);
        Serial.println("[WS] Message envoyé : " + msg);
    }
    else
    {
        Serial.println("[WS] Erreur : Non connecté au serveur Qt");
    }
}

void ConnectionRaspberryPi::cloturerSession()
{
    EnvoyerNotification("STOP;0;0");
    _webSocket.close();
    _connecte = false;
    client.stop();
}

void ConnectionRaspberryPi::maintenirConnexion()
{
    // Traite les messages et événements entrants
    _webSocket.poll();
}

void ConnectionRaspberryPi::EnvoyerAlerte(int type, String texte)
{
    JsonDocument doc;
    doc["action"] = "alerte";
    doc["type"] = type;
    doc["message"] = texte;
    doc["idBorne"] = 1;

    String trame;
    serializeJson(doc, trame);
    EnvoyerNotification(trame);
}