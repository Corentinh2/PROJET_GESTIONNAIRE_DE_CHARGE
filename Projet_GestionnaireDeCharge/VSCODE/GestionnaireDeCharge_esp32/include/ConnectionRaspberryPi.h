#ifndef CONNECTIONRASPBERRYPI_H
#define CONNECTIONRASPBERRYPI_H

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

using namespace websockets;

class ConnectionRaspberryPi {
private:
    const char* adresseRaspi = "192.168.2.90";
    const int   portRaspi    = 8080;

    WebsocketsClient _webSocket;

public:
    ConnectionRaspberryPi();
    void EnvoyerNotification(String msg);
    void cloturerSession();
    void maintenirConnexion();
    void EnvoyerAlerte(int type, String texte);
    void EnvoyerMesures(float moyennePuissance);
};

#endif