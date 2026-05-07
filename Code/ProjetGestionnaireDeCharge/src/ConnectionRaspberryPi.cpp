/**
 * @file ConnectionRaspberryPi.cpp
 * @brief Implémentation de la classe ConnectionRaspberryPi pour la communication
 *        Wi-Fi et WebSocket entre l'ESP32 et le serveur Qt du Raspberry Pi.
 *
 * Ce fichier gère la connexion Wi-Fi de l'ESP32, l'établissement et le maintien
 * d'une connexion WebSocket vers le serveur Qt, l'envoi de notifications et
 * d'alertes structurées en JSON, ainsi que la gestion des événements WebSocket.
 */

#include "ConnectionRaspberryPi.h"

/**
 * @brief Constructeur par défaut de la classe ConnectionRaspberryPi.
 *
 * Initialise l'objet sans configuration particulière. Les paramètres réseau
 * (SSID, mot de passe, adresse du Raspberry Pi) sont définis dans le fichier
 * d'en-tête associé.
 */
ConnectionRaspberryPi::ConnectionRaspberryPi() {
    // Constructeur vide ou initialisation par défaut
}

/**
 * @brief Initialise la connexion Wi-Fi et établit la connexion WebSocket.
 *
 * Se connecte au réseau Wi-Fi avec les identifiants configurés, puis affiche
 * l'adresse IP obtenue sur le port série. Une fois le Wi-Fi établi, démarre
 * la connexion WebSocket vers le serveur Qt du Raspberry Pi et enregistre
 * le gestionnaire d'événements.
 *
 * @note Cette méthode est bloquante tant que la connexion Wi-Fi n'est pas établie.
 *       Elle doit être appelée dans le setup() avant la boucle principale.
 */
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

/**
 * @brief Envoie un message texte au serveur Qt via WebSocket.
 *
 * Vérifie que la connexion WebSocket est active avant d'envoyer le message.
 * En cas d'échec de connexion, un message d'erreur est affiché sur le port série.
 *
 * @param msg  Chaîne de caractères à envoyer au serveur (ex. : trame JSON ou commande texte).
 */
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

/**
 * @brief Clôture la session en notifiant le serveur Qt et en fermant le client.
 *
 * Envoie un message de terminaison ("STOP;0;0") au serveur Qt pour signaler
 * la fin de la session, puis ferme la connexion client.
 */
void ConnectionRaspberryPi::cloturerSession() {
    EnvoyerNotification("STOP;0;0"); // Message spécial pour dire à Qt que c'est fini
    client.stop();
}

/**
 * @brief Maintient la connexion WebSocket active.
 *
 * Doit être appelée à chaque itération de la boucle principale (loop()) pour
 * traiter les événements WebSocket entrants et maintenir la connexion vivante.
 */
void ConnectionRaspberryPi::maintenirConnexion() {
    _webSocket.loop();
}

/**
 * @brief Gestionnaire des événements WebSocket.
 *
 * Callback appelé automatiquement par la bibliothèque WebSocket lors des
 * changements d'état de la connexion ou à la réception de messages.
 * Les événements gérés sont : connexion, déconnexion et réception de texte.
 *
 * @param type     Type de l'événement WebSocket (WStype_CONNECTED, WStype_DISCONNECTED, WStype_TEXT, etc.).
 * @param payload  Pointeur vers les données reçues (contenu du message pour WStype_TEXT).
 * @param length   Taille en octets du payload reçu.
 */
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

/**
 * @brief Construit et envoie une alerte structurée en JSON au serveur Qt.
 *
 * Sérialise une alerte dans un document JSON contenant l'action, le type
 * d'alerte, le message descriptif et l'identifiant de borne, puis transmet
 * la trame via EnvoyerNotification().
 *
 * @param type   Type de l'alerte : 0 pour une alerte courant, 1 pour une alerte température.
 * @param texte  Message descriptif de l'alerte (ex. : "Surcharge detectee : 19.5A").
 */
void ConnectionRaspberryPi::EnvoyerAlerte(int type, String texte) {
    StaticJsonDocument<200> doc;
    doc["action"] = "alerte";
    doc["type"] = type;      // 0 pour courant, 1 pour température
    doc["message"] = texte;
    doc["idBorne"] = 1;

    String trame;
    serializeJson(doc, trame);
    
    // On réutilise ta méthode sécurisée pour l'envoi
    EnvoyerNotification(trame);
}