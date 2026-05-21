#ifndef CONNECTIONRASPBERRYPI_H
#define CONNECTIONRASPBERRYPI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

using namespace websockets;

class ConnectionRaspberryPi {
private:
    const char* ssid        = "fortinet";
    const char* password    = "Projet2026";
    const char* adresseRaspi = "192.168.2.90";
    const int   portRaspi   = 8080;

    WiFiClient    client;
    WebsocketsClient _webSocket;

    // Flag interne pour suivre l'état de connexion
    bool _connecte = false;

public:
    ConnectionRaspberryPi();
    void initialiserConnexion();
    void EnvoyerNotification(String msg);
    void cloturerSession();
    void maintenirConnexion();
    void EnvoyerAlerte(int type, String texte);
};

#endif