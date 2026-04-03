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