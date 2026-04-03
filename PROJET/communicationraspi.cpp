#include "communicationraspi.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

CommunicationRaspi::CommunicationRaspi(QObject *parent) : QObject(parent) {

    connect(&m_webSocket, &QWebSocket::connected,
            this, &CommunicationRaspi::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected,
            this, &CommunicationRaspi::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CommunicationRaspi::onTextMessageReceived);

    QString url = QString("ws://%1:%2").arg(RASPI_IP).arg(RASPI_PORT);
    qDebug() << "[RASPI] Connexion à" << url;
    m_webSocket.open(QUrl(url));
}

CommunicationRaspi::~CommunicationRaspi() {
    m_webSocket.close();
}

// ============================================================
// CONNEXION
// ============================================================

void CommunicationRaspi::onConnected() {
    qDebug() << "[RASPI] Connecté au serveur Raspberry Pi !";
    emit connectionStatusChanged("Connecté");

    QJsonObject id;
    id["action"] = "identify";
    id["client"] = "raspi";
    m_webSocket.sendTextMessage(QJsonDocument(id).toJson(QJsonDocument::Compact));

    obtenirStation();
}

void CommunicationRaspi::onDisconnected() {
    qDebug() << "[RASPI] Déconnecté du serveur Raspberry Pi.";
    emit connectionStatusChanged("Déconnecté");
}

// ============================================================
// ENVOI DE REQUÊTES (QML → Raspi)
// ============================================================

void CommunicationRaspi::obtenirStation() {
    QJsonObject json;
    json["action"] = "obtenirStation";

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE :" << trame;
    qDebug() << "----------------------------------------";
}

void CommunicationRaspi::obtenirVehicule() {
    QJsonObject json;
    json["action"] = "obtenirVehicule";

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "----------------------------------------";
    qDebug() << "[RASPI] TRAME ENVOYÉE :" << trame;
    qDebug() << "----------------------------------------";
}

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

    // --- Réception d'une station ---
    if (action == "station") {
        int id         = json["id"].toInt();
        QString name   = json["name"].toString();
        QString kwh    = json["kwh"].toString();
        QString status = json["status"].toString();

        qDebug() << "[RASPI]   id     :" << id;
        qDebug() << "[RASPI]   name   :" << name;
        qDebug() << "[RASPI]   kwh    :" << kwh;
        qDebug() << "[RASPI]   status :" << status;

        emit stationRecue(id, name, kwh, status);
    }

    // --- Réception d'un véhicule ---
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
