/**
 * @file communicationraspi.cpp
 * @brief Implémentation de la classe CommunicationRaspi.
 * @author Coco
 * @version 1.0
 * @date Avril 2026
 */

#include "communicationraspi.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

// ============================================================
// CONSTRUCTEUR / DESTRUCTEUR
// ============================================================

/**
 * @brief Initialise la connexion WebSocket vers le Raspberry Pi.
 *
 * Configure les signaux/slots pour gérer la connexion, la déconnexion
 * et la réception des messages. Le timer de reconnexion est configuré
 * pour se déclencher toutes les 5 secondes. La connexion est ouverte
 * immédiatement à la construction.
 */
CommunicationRaspi::CommunicationRaspi(QObject *parent) : QObject(parent) {

    m_url = QString("ws://%1:%2").arg(RASPI_IP).arg(RASPI_PORT);

    connect(&m_webSocket, &QWebSocket::connected,
            this, &CommunicationRaspi::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected,
            this, &CommunicationRaspi::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CommunicationRaspi::onTextMessageReceived);

    m_reconnectTimer.setInterval(5000);
    m_reconnectTimer.setSingleShot(false);
    connect(&m_reconnectTimer, &QTimer::timeout,
            this, &CommunicationRaspi::tenterReconnexion);

    qDebug() << "[RASPI] Connexion à" << m_url;
    m_webSocket.open(QUrl(m_url));
}

/**
 * @brief Arrête le timer de reconnexion et ferme la connexion WebSocket.
 */
CommunicationRaspi::~CommunicationRaspi() {
    m_reconnectTimer.stop();
    m_webSocket.close();
}

// ============================================================
// CONNEXION / RECONNEXION
// ============================================================

/**
 * @brief Appelé lorsque la connexion WebSocket est établie.
 *
 * Arrête le timer de reconnexion, émet le signal de statut "Connecté"
 * et lance automatiquement une requête pour obtenir la liste des bornes.
 */
void CommunicationRaspi::onConnected() {
    qDebug() << "[RASPI] Connecté au serveur Raspberry Pi !";
    m_reconnectTimer.stop();
    emit connectionStatusChanged("Connecté");
    obtenirStation();
}

/**
 * @brief Appelé lorsque la connexion WebSocket est perdue.
 *
 * Émet le signal de statut "Déconnecté" et démarre le timer
 * de reconnexion automatique toutes les 5 secondes.
 */
void CommunicationRaspi::onDisconnected() {
    qDebug() << "[RASPI] Déconnecté du serveur Raspberry Pi.";
    emit connectionStatusChanged("Déconnecté");
    qDebug() << "[RASPI] Reconnexion dans 5 secondes...";
    m_reconnectTimer.start();
}

/**
 * @brief Tente de rouvrir la connexion WebSocket vers le Raspberry Pi.
 */
void CommunicationRaspi::tenterReconnexion() {
    qDebug() << "[RASPI] Tentative de reconnexion à" << m_url;
    m_webSocket.open(QUrl(m_url));
}

// ============================================================
// ENVOI DE REQUÊTES (QML → Raspi)
// ============================================================

/**
 * @brief Envoie une requête JSON au Raspberry Pi pour obtenir la liste des bornes.
 *
 * Appelée automatiquement à la connexion via onConnected(), et manuellement
 * depuis le QML lors d'une déconnexion/reconnexion de borne.
 *
 * Trame envoyée : {"action":"obtenirStation"}
 */
void CommunicationRaspi::obtenirStation() {
    QJsonObject json;
    json["action"] = "obtenirStation";

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE :" << trame;
    qDebug() << "----------------------------------------";
}

/**
 * @brief Envoie une requête JSON au Raspberry Pi pour obtenir la liste des véhicules.
 *
 * Trame envoyée : {"action":"obtenirVehicule"}
 */
void CommunicationRaspi::obtenirVehicule() {
    QJsonObject json;
    json["action"] = "obtenirVehicule";

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE :" << trame;
    qDebug() << "----------------------------------------";
}

/**
 * @brief Envoie une requête JSON au Raspberry Pi pour ajouter un véhicule.
 *
 * Vérifie que le nom du véhicule n'est pas vide avant l'envoi.
 *
 * Trame envoyée : {"action":"ajouterVehicule","name":name,"km":km}
 *
 * @param name Nom/modèle du véhicule (ex : "Renault Zoé").
 * @param km Kilométrage actuel sous forme de chaîne (converti en entier avant envoi).
 */
void CommunicationRaspi::ajouterVehicule(const QString &name, const QString &km) {
    if (name.isEmpty()) {
        qDebug() << "[RASPI] Erreur : nom du véhicule vide.";
        return;
    }

    QJsonObject json;
    json["action"] = "ajouterVehicule";
    json["name"]   = name;
    json["km"]     = km.toInt();

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE — Ajout véhicule";
    qDebug() << "[RASPI] Contenu JSON :" << trame;
    qDebug() << "[RASPI]   name :" << name;
    qDebug() << "[RASPI]   km   :" << km;
    qDebug() << "----------------------------------------";
}

/**
 * @brief Envoie une requête JSON au Raspberry Pi pour modifier le kilométrage d'un véhicule.
 *
 * Trame envoyée : {"action":"modifierKilometrage","id":id,"km":km}
 *
 * @param id Identifiant du véhicule à modifier.
 * @param km Nouveau kilométrage du véhicule.
 */
void CommunicationRaspi::modifierKilometrage(int id, int km) {
    QJsonObject json;
    json["action"] = "modifierKilometrage";
    json["id"]     = id;
    json["km"]     = km;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE — Modifier kilométrage";
    qDebug() << "[RASPI]   id :" << id;
    qDebug() << "[RASPI]   km :" << km;
    qDebug() << "----------------------------------------";
}

/**
 * @brief Envoie une requête JSON au Raspberry Pi pour supprimer un véhicule.
 *
 * Trame envoyée : {"action":"supprimerVehicule","id":id}
 *
 * @param id Identifiant du véhicule à supprimer.
 */
void CommunicationRaspi::supprimerVehicule(int id) {
    QJsonObject json;
    json["action"] = "supprimerVehicule";
    json["id"]     = id;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE — Suppression véhicule";
    qDebug() << "[RASPI] Contenu JSON :" << trame;
    qDebug() << "[RASPI]   id :" << id;
    qDebug() << "----------------------------------------";
}

// ============================================================
// RÉCEPTION DE DONNÉES (Raspi → QML)
// ============================================================

/**
 * @brief Traite les messages WebSocket reçus depuis le Raspberry Pi.
 *
 * Analyse la trame JSON reçue et émet le signal correspondant selon l'action :
 * - "station"  : émet stationRecue() avec les informations de la borne.
 * - "vehicule" : émet vehiculeRecu() avec les informations du véhicule.
 *
 * @param message Trame JSON brute reçue depuis le Raspberry Pi.
 */
void CommunicationRaspi::onTextMessageReceived(const QString &message) {
    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME REÇUE (brut) :" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (!doc.isObject()) {
        qDebug() << "[RASPI] ERREUR : Message invalide (pas un objet JSON)";
        qDebug() << "----------------------------------------";
        return;
    }

    QJsonObject json = doc.object();
    QString action = json["action"].toString();
    qDebug() << "[RASPI] Action décodée :" << action;

    if (action == "station") {
        int id         = json["id"].toInt();
        QString name   = json["name"].toString();
        QString kwh    = json["kwh"].toString();
        QString status = json["status"].toString();
        QString ip     = json["ip"].toString();

        qDebug() << "[RASPI]   id     :" << id;
        qDebug() << "[RASPI]   name   :" << name;
        qDebug() << "[RASPI]   kwh    :" << kwh;
        qDebug() << "[RASPI]   status :" << status;
        qDebug() << "[RASPI]   ip     :" << ip;

        emit stationRecue(id, name, kwh, status, ip);
    }

    if (action == "vehicule") {
        int id       = json["id"].toInt();
        QString name = json["name"].toString();
        QString km   = QString::number(json["km"].toInt());

        qDebug() << "[RASPI]   id   :" << id;
        qDebug() << "[RASPI]   name :" << name;
        qDebug() << "[RASPI]   km   :" << km;

        emit vehiculeRecu(id, name, km);
    }

    qDebug() << "----------------------------------------";
}
