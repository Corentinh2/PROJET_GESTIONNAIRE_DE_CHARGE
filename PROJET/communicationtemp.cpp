#include "communicationtemp.h"
#include <QDebug>
#include <QStringList>

CommunicationTemp::CommunicationTemp(QObject *parent) : QObject(parent) {
    // Écouteurs en mode BINAIRE
    connect(&m_webSocket, &QWebSocket::connected, this, &CommunicationTemp::onConnected);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &CommunicationTemp::onBinaryMessageReceived);

    m_webSocket.open(QUrl("ws://172.18.58.97:1234"));
}

CommunicationTemp::~CommunicationTemp() {
    m_webSocket.close();
}

void CommunicationTemp::onConnected() {
    qDebug() << "Application connectée au Serveur !";
    emit connectionStatusChanged("Connecté");

    // Le tuyau est branché, on demande TOUTES les données !
    requestStationsList();
    requestVehiclesList();
    requestSessionsHistory();

    return;
}

void CommunicationTemp::requestStationsList() {
    QByteArray trame;
    trame.append(static_cast<char>(0x02));
    trame.append('B');
    m_webSocket.sendBinaryMessage(trame);

    return;
}

void CommunicationTemp::requestVehiclesList() {
    QByteArray trame;
    trame.append(static_cast<char>(0x02));
    trame.append('V');
    m_webSocket.sendBinaryMessage(trame);

    return;
}

void CommunicationTemp::requestSessionsHistory() {
    QByteArray trame;
    trame.append(static_cast<char>(0x02));
    trame.append('H');
    m_webSocket.sendBinaryMessage(trame);

    return;
}

void CommunicationTemp::addVehicleToDb(const QString &name, const QString &km) {
    QByteArray trame;
    bool donneesValides = true;

    if (name.isEmpty()) {
        donneesValides = false;
    }

    if (donneesValides) {
        trame.append(static_cast<char>(0x00));
        trame.append('A');

        quint32 kmValue = km.toUInt();
        trame.append(static_cast<char>((kmValue >> 24) & 0xFF));
        trame.append(static_cast<char>((kmValue >> 16) & 0xFF));
        trame.append(static_cast<char>((kmValue >> 8) & 0xFF));
        trame.append(static_cast<char>(kmValue & 0xFF));

        trame.append(name.toUtf8());
        trame.append('\0');

        trame[0] = static_cast<char>(trame.size());

        m_webSocket.sendBinaryMessage(trame);

        qDebug() << "Ordre d'ajout envoyé au serveur. En attente de confirmation...";
    }

    return;
}

void CommunicationTemp::programmerCharge(const QString &borne, const QString &vehicule, const QString &jours, const QString &start, const QString &end) {
    bool isValid = true;

    if (borne.isEmpty()) {
        isValid = false;
    }
    if (vehicule.isEmpty()) {
        isValid = false;
    }

    if (isValid) {
        // --- 1. TRAME RASPI ('P') ---
        QByteArray trameP;
        trameP.append(static_cast<char>(0x00));
        trameP.append('P');

        trameP.append(borne.toUtf8()); trameP.append('\0');
        trameP.append(vehicule.toUtf8()); trameP.append('\0');
        trameP.append(jours.toUtf8()); trameP.append('\0');
        trameP.append(start.toUtf8()); trameP.append('\0');
        trameP.append(end.toUtf8()); trameP.append('\0');

        trameP[0] = static_cast<char>(trameP.size());

        m_webSocket.sendBinaryMessage(trameP);
        qDebug() << "[MOBILE] Trame Raspi 'P' envoyée";

        // --- 2. TRAME ESP ('C') ---
        QByteArray trameC;
        trameC.append(static_cast<char>(0x07));
        trameC.append('C');

        quint8 masqueJours = 0;
        if (jours.contains("Lun")) { masqueJours += 1; }
        if (jours.contains("Mar")) { masqueJours += 2; }
        if (jours.contains("Mer")) { masqueJours += 4; }
        if (jours.contains("Jeu")) { masqueJours += 8; }
        if (jours.contains("Ven")) { masqueJours += 16; }
        if (jours.contains("Sam")) { masqueJours += 32; }
        if (jours.contains("Dim")) { masqueJours += 64; }

        trameC.append(static_cast<char>(masqueJours));

        QString cleanStart = start.left(5);
        QString cleanEnd = end.left(5);

        QStringList startParts = cleanStart.split(":");
        QStringList endParts = cleanEnd.split(":");

        bool heuresValides = true;
        if (startParts.size() != 2) { heuresValides = false; }
        if (endParts.size() != 2) { heuresValides = false; }

        if (heuresValides) {
            quint8 hd = static_cast<quint8>(startParts[0].toUInt());
            quint8 md = static_cast<quint8>(startParts[1].toUInt());
            quint8 hf = static_cast<quint8>(endParts[0].toUInt());
            quint8 mf = static_cast<quint8>(endParts[1].toUInt());

            trameC.append(static_cast<char>(hd));
            trameC.append(static_cast<char>(md));
            trameC.append(static_cast<char>(hf));
            trameC.append(static_cast<char>(mf));

            m_webSocket.sendBinaryMessage(trameC);
            qDebug() << "[MOBILE] Trame ESP 'C' envoyée";
        }
    }

    return;
}

void CommunicationTemp::onBinaryMessageReceived(QByteArray message) {
    bool isValid = (message.size() >= 2);

    if (isValid) {
        char cmd = message[1];

        // DÉCODAGE TRAME BORNE ('b')
        if (cmd == 'b') {
            quint8 msb = static_cast<quint8>(message[2]);
            quint8 lsb = static_cast<quint8>(message[3]);
            quint16 puissance = (msb << 8) | lsb;

            int indexNom = 4;
            int finNom = message.indexOf('\0', indexNom);
            QString nom = QString::fromUtf8(message.mid(indexNom, finNom - indexNom));

            int indexEmp = finNom + 1;
            int finEmp = message.indexOf('\0', indexEmp);
            QString emplacement = QString::fromUtf8(message.mid(indexEmp, finEmp - indexEmp));

            QString kwhStr = QString::number(puissance) + " kW - " + emplacement;
            QString statusStr = "Disponible";

            emit stationReceived(nom, kwhStr, statusStr);
        }

        // DÉCODAGE TRAME VÉHICULE ('v')
        if (cmd == 'v') {
            quint8 b1 = static_cast<quint8>(message[2]);
            quint8 b2 = static_cast<quint8>(message[3]);
            quint8 b3 = static_cast<quint8>(message[4]);
            quint8 b4 = static_cast<quint8>(message[5]);
            quint32 km = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;

            int indexNom = 6;
            int finNom = message.indexOf('\0', indexNom);
            QString nom = QString::fromUtf8(message.mid(indexNom, finNom - indexNom));

            QString kmStr = QString::number(km);

            emit vehicleReceived(nom, kmStr);
        }

        // DÉCODAGE TRAME SESSION ('s')
        if (cmd == 's') {
            int indexBorne = 2;
            int finBorne = message.indexOf('\0', indexBorne);
            QString borne = QString::fromUtf8(message.mid(indexBorne, finBorne - indexBorne));

            int indexVehicule = finBorne + 1;
            int finVehicule = message.indexOf('\0', indexVehicule);
            QString vehicule = QString::fromUtf8(message.mid(indexVehicule, finVehicule - indexVehicule));

            int indexJours = finVehicule + 1;
            int finJours = message.indexOf('\0', indexJours);
            QString jours = QString::fromUtf8(message.mid(indexJours, finJours - indexJours));

            int indexStart = finJours + 1;
            int finStart = message.indexOf('\0', indexStart);
            QString start = QString::fromUtf8(message.mid(indexStart, finStart - indexStart));

            int indexEnd = finStart + 1;
            int finEnd = message.indexOf('\0', indexEnd);
            QString end = QString::fromUtf8(message.mid(indexEnd, finEnd - indexEnd));

            emit sessionReceived(borne, vehicule, jours, start, end);
        }
    }

    return;
}
