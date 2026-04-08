#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char* SSID     = "fortinet";
const char* PASSWORD = "Projet2026";

WebSocketsServer wsServer(81); // Port 81

void onClientEvent(uint8_t clientId, WStype_t type,
                   uint8_t* payload, size_t length) {
    switch (type) {

        case WStype_DISCONNECTED:
            Serial.printf("[WS] Client #%u déconnecté\n", clientId);
            break;

        case WStype_CONNECTED: {
            IPAddress ip = wsServer.remoteIP(clientId);
            Serial.printf("[WS] Client #%u connecté depuis %s\n",
                          clientId, ip.toString().c_str());
            wsServer.sendTXT(clientId, "{\"status\":\"connected\"}");
            break;
        }

        case WStype_TEXT: {
            Serial.printf("[WS] Client #%u → %s\n", clientId, payload);

            // Parse JSON entrant
            JsonDocument doc;
            if (!deserializeJson(doc, payload)) {
                const char* cmd = doc["cmd"];
                if (strcmp(cmd, "ping") == 0) {
                    wsServer.sendTXT(clientId, "{\"cmd\":\"pong\"}");
                }
            }
            break;
        }

        case WStype_BIN:
            Serial.printf("[WS] Binaire reçu de #%u (%u bytes)\n",
                          clientId, length);
            // Renvoyer en écho à tous
            wsServer.broadcastBIN(payload, length);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("=== Démarrage ESP32 ===");

    // Connexion WiFi
    WiFi.begin(SSID, PASSWORD);
    Serial.print("Connexion WiFi");

    int tentatives = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        tentatives++;
        if (tentatives > 20) {
            Serial.println("\nÉchec connexion WiFi !");
            Serial.println("Vérifie SSID et PASSWORD");
            return;
        }
    }

    Serial.println("\nWiFi connecté !");
    Serial.print("IP de l'ESP32 : ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal (RSSI) : ");
    Serial.println(WiFi.RSSI());

    // Démarrage serveur WebSocket
    wsServer.begin();
    wsServer.onEvent(onClientEvent);
    Serial.println("Serveur WebSocket démarré !");
    Serial.print("Connecte-toi sur ws://");
    Serial.print(WiFi.localIP());
    Serial.println(":81");
    Serial.println("======================");
}


void loop() {
    wsServer.loop();
}