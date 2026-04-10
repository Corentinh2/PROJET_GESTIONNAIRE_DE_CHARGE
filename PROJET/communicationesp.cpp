/**
 * @file communicationesp.cpp
 * @brief Implémentation de la classe CommunicationEsp.
 * @author Coco
 * @version 1.0
 * @date Avril 2026
 */

#include "communicationesp.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

// ============================================================
// CONSTRUCTEUR / DESTRUCTEUR
// ============================================================

/**
 * @brief Initialise la connexion WebSocket et le timer de reconnexion.
 *
 * Configure les signaux/slots pour gérer la connexion, la déconnexion
 * et la réception des messages. Le timer de reconnexion est configuré
 * pour se déclencher toutes les 5 secondes.
 */
CommunicationEsp::CommunicationEsp(QObject *parent) : QObject(parent) {

    m_attenteNouveauListe = false;
    m_url = "";

    connect(&m_webSocket, &QWebSocket::connected,
            this, &CommunicationEsp::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected,
            this, &CommunicationEsp::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &CommunicationEsp::onTextMessageReceived);

    m_reconnectTimer.setInterval(5000);
    m_reconnectTimer.setSingleShot(false);
    connect(&m_reconnectTimer, &QTimer::timeout,
            this, &CommunicationEsp::tenterReconnexion);
}

/**
 * @brief Arrête le timer de reconnexion et ferme la connexion WebSocket.
 */
CommunicationEsp::~CommunicationEsp() {
    m_reconnectTimer.stop();
    m_webSocket.close();
}

// ============================================================
// GETTERS / SETTERS
// ============================================================

/**
 * @brief Retourne le nom de la borne actuellement active.
 * @return Nom de la borne active.
 */
QString CommunicationEsp::activeBorne() const {
    return m_activeBorne;
}

/**
 * @brief Définit l'adresse IP de l'ESP32 et ouvre la connexion WebSocket.
 *
 * Ferme la connexion existante avant d'en ouvrir une nouvelle
 * avec la nouvelle adresse IP fournie.
 *
 * @param ip Adresse IP de l'ESP32 (ex : "192.168.1.50").
 */
void CommunicationEsp::setIp(const QString &ip) {
    m_url = QString("ws://%1:%2").arg(ip).arg(ESP_PORT);
    qDebug() << "[ESP32] Changement IP →" << m_url;
    m_webSocket.close();
    m_webSocket.open(QUrl(m_url));
}

/**
 * @brief Définit le nom de la borne active et émet le signal de changement.
 * @param borne Nom de la borne.
 */
void CommunicationEsp::setActiveBorne(const QString &borne) {
    if (m_activeBorne != borne) {
        m_activeBorne = borne;
        emit activeBorneChanged();
    }
}

/**
 * @brief Retourne le nom du véhicule actuellement sélectionné.
 * @return Nom du véhicule actif.
 */
QString CommunicationEsp::activeVehicule() const {
    return m_activeVehicule;
}

/**
 * @brief Définit le nom du véhicule actif et émet le signal de changement.
 * @param vehicule Nom du véhicule.
 */
void CommunicationEsp::setActiveVehicule(const QString &vehicule) {
    if (m_activeVehicule != vehicule) {
        m_activeVehicule = vehicule;
        emit activeVehiculeChanged();
    }
}

// ============================================================
// UTILITAIRES : TEXTE → COMPACT (envoi vers ESP)
// ============================================================

/**
 * @brief Convertit une chaîne de jours en masque binaire.
 *
 * Chaque bit du masque correspond à un jour de la semaine :
 * - Bit 0 (valeur 1)  : Lundi
 * - Bit 1 (valeur 2)  : Mardi
 * - Bit 2 (valeur 4)  : Mercredi
 * - Bit 3 (valeur 8)  : Jeudi
 * - Bit 4 (valeur 16) : Vendredi
 * - Bit 5 (valeur 32) : Samedi
 * - Bit 6 (valeur 64) : Dimanche
 *
 * @param jours Jours sous forme de texte (ex : "Lun, Mar, Dim").
 * @return Masque binaire représentant les jours sélectionnés.
 */
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

/**
 * @brief Extrait les heures et minutes depuis une chaîne au format "HH:MM".
 *
 * Ignore le suffixe " (lendemain)" si présent.
 * Retourne 0 pour heure et minute si le format est invalide.
 *
 * @param heureStr Chaîne de temps (ex : "08:30" ou "09:00 (lendemain)").
 * @param heure Référence recevant l'heure extraite.
 * @param minute Référence recevant les minutes extraites.
 */
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

/**
 * @brief Convertit un masque binaire de jours en chaîne lisible.
 *
 * @param masque Masque binaire (bit 0 = Lundi, bit 6 = Dimanche).
 * @return Chaîne de jours séparés par des virgules (ex : "Lun, Mar, Dim").
 */
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

/**
 * @brief Formate une heure et des minutes en chaîne "HH:MM".
 *
 * @param heure Heure (0-23).
 * @param minute Minutes (0-59).
 * @return Chaîne formatée avec zéros initiaux (ex : "08:05").
 */
QString CommunicationEsp::formaterHeure(int heure, int minute) {
    return QString("%1:%2")
            .arg(heure, 2, 10, QChar('0'))
            .arg(minute, 2, 10, QChar('0'));
}

// ============================================================
// CONNEXION / RECONNEXION
// ============================================================

/**
 * @brief Appelé lorsque la connexion WebSocket est établie.
 *
 * Arrête le timer de reconnexion et émet le signal de changement de statut.
 */
void CommunicationEsp::onConnected() {
    qDebug() << "[ESP32] Connecté au boîtier ESP32 !";
    m_reconnectTimer.stop();
    emit connectionStatusChanged("Connecté");
}

/**
 * @brief Appelé lorsque la connexion WebSocket est perdue.
 *
 * Émet le signal de changement de statut et démarre le timer
 * de reconnexion automatique toutes les 5 secondes.
 */
void CommunicationEsp::onDisconnected() {
    qDebug() << "[ESP32] Déconnecté du boîtier ESP32.";
    emit connectionStatusChanged("Déconnecté");
    qDebug() << "[ESP32] Reconnexion dans 5 secondes...";
    m_reconnectTimer.start();
}

/**
 * @brief Tente de rouvrir la connexion WebSocket vers l'ESP32.
 *
 * Ne fait rien si l'URL n'a pas encore été définie via setIp().
 */
void CommunicationEsp::tenterReconnexion() {
    if (m_url.isEmpty()) { return; }
    qDebug() << "[ESP32] Tentative de reconnexion à" << m_url;
    m_webSocket.open(QUrl(m_url));
}

// ============================================================
// CALENDRIERS DE CHARGE (QML → ESP32)
// ============================================================

/**
 * @brief Envoie une requête JSON à l'ESP32 pour obtenir la liste des calendriers.
 *
 * Active le flag m_attenteNouveauListe pour que la réception du premier
 * calendrier déclenche un clear de la liste côté QML.
 *
 * Trame envoyée : {"action":"obtenirCalendrier"}
 */
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

/**
 * @brief Envoie une requête JSON à l'ESP32 pour ajouter un calendrier de charge.
 *
 * Convertit les jours en masque binaire et extrait les heures/minutes
 * avant d'envoyer la trame JSON.
 *
 * Trame envoyée : {"action":"ajouterCalendrier","jours":masque,"hd":h,"md":m,"hf":h,"mf":m}
 *
 * @param jours Jours sélectionnés (ex : "Lun, Mar, Mer").
 * @param start Heure de début au format "HH:MM".
 * @param end Heure de fin au format "HH:MM" (peut contenir " (lendemain)").
 */
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

/**
 * @brief Envoie une requête JSON à l'ESP32 pour supprimer un calendrier.
 *
 * Trame envoyée : {"action":"supprimerCalendrier","id":id}
 *
 * @param id Identifiant du calendrier à supprimer.
 */
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

/**
 * @brief Envoie une commande de marche forcée à l'ESP32.
 *
 * Permet d'activer ou désactiver manuellement le relais de charge,
 * indépendamment des calendriers programmés.
 *
 * Trame envoyée : {"action":"marcheForcee","activer":true/false}
 *
 * @param activer true pour activer le relais, false pour le désactiver.
 */
void CommunicationEsp::marcheForcee(bool activer) {
    QJsonObject json;
    json["action"]  = "marcheForcee";
    json["activer"] = activer;

    QString trame = QJsonDocument(json).toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(trame);

    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME ENVOYÉE — Marche forcée";
    qDebug() << "[ESP32] Contenu JSON :" << trame;
    if (activer) {
        qDebug() << "[ESP32]   activer : true (ON)";
    } else {
        qDebug() << "[ESP32]   activer : false (OFF)";
    }
    qDebug() << "========================================";
}

// ============================================================
// RÉCEPTION DE DONNÉES (ESP32 → QML)
// ============================================================

/**
 * @brief Traite les messages WebSocket reçus depuis l'ESP32.
 *
 * Les messages peuvent contenir plusieurs trames JSON séparées par des
 * sauts de ligne. Chaque trame est analysée individuellement.
 *
 * Actions traitées :
 * - "calendrier" : émet clearCalendriers() si nouvelle liste attendue,
 *                  puis calendrierRecu() si le masque de jours est valide.
 * - "alerte"     : émet alerteRecue() avec le type et le message.
 *
 * @param message Trame(s) JSON brute(s) reçue(s) depuis l'ESP32.
 */
void CommunicationEsp::onTextMessageReceived(const QString &message) {
    qDebug() << "========================================";
    qDebug() << "[ESP32] TRAME REÇUE (brut) :" << message;

    QStringList trames = message.split('\n', Qt::SkipEmptyParts);

    for (const QString &trame : trames) {
        QJsonDocument doc = QJsonDocument::fromJson(trame.trimmed().toUtf8());

        if (!doc.isObject()) {
            qDebug() << "[ESP32] ERREUR : Message invalide (pas un objet JSON) :" << trame;
            continue;
        }

        QJsonObject json = doc.object();
        QString action = json["action"].toString();
        qDebug() << "[ESP32] Action décodée :" << action;

        if (action == "calendrier") {
            int id     = json["id"].toInt();
            int masque = json["jours"].toInt();
            int hd     = json["hd"].toInt();
            int md     = json["md"].toInt();
            int hf     = json["hf"].toInt();
            int mf     = json["mf"].toInt();

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

            if (m_attenteNouveauListe) {
                emit clearCalendriers();
                m_attenteNouveauListe = false;
                qDebug() << "[ESP32] Clear calendriers (nouvelle liste)";
            }

            if (masque > 0) {
                emit calendrierRecu(id, borne, vehicule, jours, start, end);
                qDebug() << "[ESP32] Calendrier valide, transmis au QML";
            } else {
                qDebug() << "[ESP32] Calendrier ignoré (masque jours vide)";
            }
        }

        if (action == "alerte") {
            QString type = json["type"].toString();
            QString msg  = json["message"].toString();

            qDebug() << "[ESP32]   type    :" << type;
            qDebug() << "[ESP32]   message :" << msg;

            emit alerteRecue(type, msg);
        }

        qDebug() << "========================================";
    }
}
