#include "communicationesp.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

CommunicationEsp::CommunicationEsp(QObject *parent) : QObject(parent) {

    m_attenteNouveauListe = false;

    connect(&m_webSocket, &QWebSocket::connected,
            this, &CommunicationEsp::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected,
            this, &CommunicationEsp::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CommunicationEsp::onTextMessageReceived);

    QString url = QString("ws://%1:%2").arg(ESP_IP).arg(ESP_PORT);
    qDebug() << "[ESP32] Connexion à" << url;
    m_webSocket.open(QUrl(url));
}

CommunicationEsp::~CommunicationEsp() {
    m_webSocket.close();
}

// ============================================================
// GETTERS / SETTERS
// ============================================================

QString CommunicationEsp::activeBorne() const {
    return m_activeBorne;
}

void CommunicationEsp::setActiveBorne(const QString &borne) {
    if (m_activeBorne != borne) {
        m_activeBorne = borne;
        emit activeBorneChanged();
    }
}

QString CommunicationEsp::activeVehicule() const {
    return m_activeVehicule;
}

void CommunicationEsp::setActiveVehicule(const QString &vehicule) {
    if (m_activeVehicule != vehicule) {
        m_activeVehicule = vehicule;
        emit activeVehiculeChanged();
    }
}

// ============================================================
// UTILITAIRES : TEXTE → COMPACT (envoi vers ESP)
// ============================================================

int CommunicationEsp::joursVersmasque(const QString &jours) {
    int masque = 0;

    if (jours.contains("Lun")) { masque += 1; }
    if (jours.contains("Mar")) { masque += 2; }
    if (jours.contains("Mer")) { masque += 4; }
    if (jours.contains("Jeu")) { masque += 8; }
    if (jours.contains("Ven")) { masque += 16; }
    if (jours.contains("Sam")) { masque += 32; }
    if (jours.contains("Dim")) { masque += 64; }

    return masque;
}

void CommunicationEsp::extraireHeureMinute(const QString &heureStr, int &heure, int &minute) {
    QString clean = heureStr.left(5);
    QStringList parts = clean.split(":");

    heure = 0;
    minute = 0;

    if (parts.size() == 2) {
        heure = parts[0].toInt();
        minute = parts[1].toInt();
    }
}

// ============================================================
// UTILITAIRES : COMPACT → TEXTE (réception depuis ESP)
// ============================================================

QString CommunicationEsp::masqueVersJours(int masque) {
    QStringList jours;

    if (masque & 1)  { jours.append("Lun"); }
    if (masque & 2)  { jours.append("Mar"); }
    if (masque & 4)  { jours.append("Mer"); }
    if (masque & 8)  { jours.append("Jeu"); }
    if (masque & 16) { jours.append("Ven"); }
    if (masque & 32) { jours.append("Sam"); }
    if (masque & 64) { jours.append("Dim"); }

    return jours.join(", ");
}

QString CommunicationEsp::formaterHeure(int heure, int minute) {
    return QString("%1:%2")
        .arg(heure, 2, 10, QChar('0'))
        .arg(minute, 2, 10, QChar('0'));
}

// ============================================================
// CONNEXION
// ============================================================

void CommunicationEsp::onConnected() {
    qDebug() << "[ESP32] Connecté au boîtier ESP32 !";
    emit connectionStatusChanged("Connecté");
}

void CommunicationEsp::onDisconnected() {
    qDebug() << "[ESP32] Déconnecté du boîtier ESP32.";
    emit connectionStatusChanged("Déconnecté");
}

// ============================================================
// CALENDRIERS DE CHARGE (QML → ESP32)
// ============================================================

void CommunicationEsp::obtenirCalendrier() {
    m_attenteNouveauListe = true;

    QJsonObject json;
    json["action"] = "obtenirCalendrier";

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME ENVOYÉE :" << trame;
    qDebug() << "========================================";
}

void CommunicationEsp::ajouterCalendrier(const QString &jours, const QString &start,
                                           const QString &end) {
    m_attenteNouveauListe = true;

    int masqueJours = joursVersmasque(jours);
    int hd = 0, md = 0, hf = 0, mf = 0;
    extraireHeureMinute(start, hd, md);
    extraireHeureMinute(end, hf, mf);

    QJsonObject json;
    json["action"] = "ajouterCalendrier";
    json["jours"]  = masqueJours;
    json["hd"]     = hd;
    json["md"]     = md;
    json["hf"]     = hf;
    json["mf"]     = mf;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME ENVOYÉE — Ajout calendrier";
    qDebug() << "[ESP32] Contenu JSON :" << trame;
    qDebug() << "[ESP32]   jours (masque) :" << masqueJours << "(" << jours << ")";
    qDebug() << "[ESP32]   début          :" << hd << "h" << md;
    qDebug() << "[ESP32]   fin            :" << hf << "h" << mf;
    qDebug() << "========================================";
}

void CommunicationEsp::supprimerCalendrier(int id) {
    m_attenteNouveauListe = true;

    QJsonObject json;
    json["action"] = "supprimerCalendrier";
    json["id"]     = id;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME ENVOYÉE — Suppression calendrier";
    qDebug() << "[ESP32] Contenu JSON :" << trame;
    qDebug() << "[ESP32]   id :" << id;
    qDebug() << "========================================";
}

// ============================================================
// COMMANDE DE PILOTAGE (QML → ESP32)
// ============================================================

void CommunicationEsp::marcheForcee(bool activer) {
    QJsonObject json;
    json["action"]  = "marcheForcee";
    json["activer"] = activer;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME ENVOYÉE — Marche forcée";
    qDebug() << "[ESP32] Contenu JSON :" << trame;
    qDebug() << "[ESP32]   activer :" << (activer ? "true (ON)" : "false (OFF)");
    qDebug() << "========================================";
}

// ============================================================
// RÉCEPTION DE DONNÉES (ESP32 → QML)
// ============================================================

void CommunicationEsp::onTextMessageReceived(const QString &message) {
    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME REÇUE (brut) :" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (!doc.isObject()) {
        qDebug() << "[ESP32] ERREUR : Message invalide (pas un objet JSON)";
        qDebug() << "========================================";
        return;
    }

    QJsonObject json = doc.object();
    QString action = json["action"].toString();
    qDebug() << "[ESP32] Action décodée :" << action;

    // --- Réception d'un calendrier (format compact) ---
    if (action == "calendrier") {
        int id     = json["id"].toInt();
        int masque = json["jours"].toInt();
        int hd     = json["hd"].toInt();
        int md     = json["md"].toInt();
        int hf     = json["hf"].toInt();
        int mf     = json["mf"].toInt();

        // Conversion compact → texte lisible
        QString jours = masqueVersJours(masque);
        QString start = formaterHeure(hd, md);
        QString end   = formaterHeure(hf, mf);

        if (hf < hd) {
            end = end + " (lendemain)";
        }

        QString borne    = m_activeBorne;
        QString vehicule = m_activeVehicule;

        qDebug() << "[ESP32]   id             :" << id;
        qDebug() << "[ESP32]   jours (masque) :" << masque << "→" << jours;
        qDebug() << "[ESP32]   début          :" << start;
        qDebug() << "[ESP32]   fin            :" << end;
        qDebug() << "[ESP32]   borne (locale) :" << borne;
        qDebug() << "[ESP32]   vehicule (local):" << vehicule;

        if (masque > 0) {
            // Si c'est le premier calendrier d'une nouvelle liste, on clear
            if (m_attenteNouveauListe) {
                emit clearCalendriers();
                m_attenteNouveauListe = false;
                qDebug() << "[ESP32] Clear calendriers (nouvelle liste)";
            }

            emit calendrierRecu(id, borne, vehicule, jours, start, end);
            qDebug() << "[ESP32] Calendrier valide, transmis au QML";
        } else {
            qDebug() << "[ESP32] Calendrier ignoré (masque jours vide)";
        }
    }

    // --- Réception d'une alerte ---
    if (action == "alerte") {
        QString type = json["type"].toString();
        QString msg  = json["message"].toString();

        qDebug() << "[ESP32]   type    :" << type;
        qDebug() << "[ESP32]   message :" << msg;

        emit alerteRecue(type, msg);
    }

    qDebug() << "========================================";
}
