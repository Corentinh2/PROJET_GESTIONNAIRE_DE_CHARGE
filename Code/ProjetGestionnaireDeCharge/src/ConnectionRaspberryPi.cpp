#include "ConnectionRaspberryPi.h"

ConnectionRaspberryPi::ConnectionRaspberryPi() {
    // Constructeur vide ou initialisation par défaut
}

void ConnectionRaspberryPi::initialiserConnexion() {
    WiFi.begin(ssid, password);
    Serial.print("Connexion au Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Affichage de la réussite et de l'IP
    Serial.println("\nConnecté !");
    Serial.print("Adresse IP de l'ESP32 : ");
    Serial.println(WiFi.localIP());

    // ← C'est ici qu'on démarre la connexion WebSocket vers la Raspi
    _webSocket.begin(adresseRaspi, portRaspi, "/");
    _webSocket.onEvent(webSocketEvent);
    Serial.println("[WS] Tentative de connexion au serveur Qt...");
}

void ConnectionRaspberryPi::EnvoyerNotification(String msg)
{
   // On vérifie si le WebSocket est prêt avant d'envoyer
    if (_webSocket.isConnected()) {
        _webSocket.sendTXT(msg); 
        Serial.println("[WS] Message envoyé : " + msg);
    } else {
        Serial.println("[WS] Erreur : Non connecté au serveur Qt");
    }
}

void ConnectionRaspberryPi::cloturerSession() {
    EnvoyerNotification("STOP;0;0"); // Message spécial pour dire à Qt que c'est fini
    client.stop();
}

void ConnectionRaspberryPi::loop() {
    _webSocket.loop();
}

void ConnectionRaspberryPi::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_CONNECTED:
            Serial.println("[WS] Connecté au serveur Qt !");
            break;
        case WStype_DISCONNECTED:
            Serial.println("[WS] Déconnecté du serveur Qt !");
            break;
        case WStype_TEXT:
            Serial.println("[WS] Message reçu : " + String((char*)payload));
            break;
    }
}