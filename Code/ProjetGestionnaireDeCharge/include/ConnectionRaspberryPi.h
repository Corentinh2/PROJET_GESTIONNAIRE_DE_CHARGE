#ifndef CONNECTIONRASPBERRYPI_H
#define CONNECTIONRASPBERRYPI_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebSocketsClient.h>

class ConnectionRaspberryPi {
private:
    const char* ssid = "fortinet";
    const char* password = "Projet2026";
    const char* adresseRaspi = "192.168.2.90"; // L'IP de ta Raspi (Qt)
    const int portRaspi = 8080;              // Le port que tu écoutes sur Qt
    WiFiClient client;
    WebSocketsClient _webSocket;

public:
    ConnectionRaspberryPi();           
    void initialiserConnexion();      // Présent sur ton diagramme
    void EnvoyerNotification(String msg); // Présent sur ton diagramme
    void cloturerSession();
    void loop();           // Présent sur ton diagramme
};
#endif