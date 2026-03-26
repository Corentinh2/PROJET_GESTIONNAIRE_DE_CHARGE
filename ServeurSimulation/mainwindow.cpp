#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- CHARGEMENT DES VÉHICULES AU DÉMARRAGE ---
    QFile fichier("vehicules.txt");
    if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream flux(&fichier);
        while (!flux.atEnd()) {
            QString ligne = flux.readLine();
            QStringList parts = ligne.split(";");
            if (parts.size() == 2) {
                ui->listVehicules->addItem("🚗 " + parts[0] + " — " + parts[1] + " km");
            }
        }
        fichier.close();
    }

    // --- CHARGEMENT DES SESSIONS AU DÉMARRAGE (Avec Tri) ---
    QFile fichierSessions("sessions.txt");
    if (fichierSessions.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream fluxS(&fichierSessions);
        while (!fluxS.atEnd()) {
            QString ligne = fluxS.readLine();
            QStringList parts = ligne.split(";");

            // Format attendu : Borne(0);Vehicule(1);Jours(2);Start(3);End(4)
            if (parts.size() == 5) {
                QString nomBorne = parts[0];
                QString affichage = "📅 " + parts[2] + " (" + parts[3] + "-" + parts[4] + ") | " + parts[1];

                // On trie selon le nom de la borne
                if (nomBorne == "Borne Camille") {
                    ui->listSessionsCamille->addItem(affichage);
                }
                if (nomBorne == "Borne Direction") {
                    ui->listSessionsDirection->addItem(affichage);
                }
            }
        }
        fichierSessions.close();
    }

    // --- INITIALISATION DU SERVEUR ---
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Serveur Simulation Raspi"),
                                              QWebSocketServer::NonSecureMode, this);

    bool isListening = m_pWebSocketServer->listen(QHostAddress::Any, 1234);

    if (isListening) {
        qDebug() << "Serveur de simulation DEMARRE sur le port 1234 !";
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &MainWindow::onNewConnection);
    }
}

MainWindow::~MainWindow()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
    delete ui;
}

void MainWindow::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &MainWindow::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);

    m_clients << pSocket;
    qDebug() << "[SERVEUR] Un client mobile vient de se connecter !";
}

void MainWindow::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    bool isValid = (message.size() >= 2);

    if (isValid) {
        char cmd = message[1];

        // --- 1. DEMANDE DES BORNES ---
        if (cmd == 'B') {
            qDebug() << "[SERVEUR] Trame demande BORNES reçue.";

            // Envoi de la Borne 1
            QByteArray rep1;
            rep1.append(static_cast<char>(0x00));
            rep1.append('b');
            quint16 p1 = 22;
            rep1.append(static_cast<char>((p1 >> 8) & 0xFF));
            rep1.append(static_cast<char>(p1 & 0xFF));
            rep1.append(QString("Borne Camille").toUtf8());
            rep1.append('\0');
            rep1.append(QString("Parking Nord").toUtf8());
            rep1.append('\0');
            rep1[0] = static_cast<char>(rep1.size());
            if (pClient) { pClient->sendBinaryMessage(rep1); }

            // Envoi de la Borne 2
            QByteArray rep2;
            rep2.append(static_cast<char>(0x00));
            rep2.append('b');
            quint16 p2 = 50;
            rep2.append(static_cast<char>((p2 >> 8) & 0xFF));
            rep2.append(static_cast<char>(p2 & 0xFF));
            rep2.append(QString("Borne Direction").toUtf8());
            rep2.append('\0');
            rep2.append(QString("Parking Sud").toUtf8());
            rep2.append('\0');
            rep2[0] = static_cast<char>(rep2.size());
            if (pClient) { pClient->sendBinaryMessage(rep2); }

            qDebug() << "[SERVEUR] Trames des bornes envoyées.";
        }

        // --- 2. DEMANDE DES VÉHICULES ---
        if (cmd == 'V') {
            qDebug() << "[SERVEUR] Trame demande VEHICULES reçue.";
            QFile fichier("vehicules.txt");
            if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream flux(&fichier);
                while (!flux.atEnd()) {
                    QString ligne = flux.readLine();
                    QStringList parts = ligne.split(";");
                    if (parts.size() == 2) {
                        QString nom = parts[0];
                        quint32 km = parts[1].toUInt();

                        QByteArray repV;
                        repV.append(static_cast<char>(0x00));
                        repV.append('v');
                        repV.append(static_cast<char>((km >> 24) & 0xFF));
                        repV.append(static_cast<char>((km >> 16) & 0xFF));
                        repV.append(static_cast<char>((km >> 8) & 0xFF));
                        repV.append(static_cast<char>(km & 0xFF));
                        repV.append(nom.toUtf8());
                        repV.append('\0');
                        repV[0] = static_cast<char>(repV.size());

                        if (pClient) { pClient->sendBinaryMessage(repV); }
                    }
                }
                fichier.close();
            }
            qDebug() << "[SERVEUR] Liste des véhicules envoyée.";
        }

        // --- 3. AJOUT D'UN VÉHICULE ---
        if (cmd == 'A') {
            qDebug() << "[SERVEUR] Trame AJOUT VEHICULE reçue.";
            quint8 b1 = static_cast<quint8>(message[2]);
            quint8 b2 = static_cast<quint8>(message[3]);
            quint8 b3 = static_cast<quint8>(message[4]);
            quint8 b4 = static_cast<quint8>(message[5]);
            quint32 kmRecu = (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;

            int indexNom = 6;
            int finNom = message.indexOf('\0', indexNom);
            QString nomRecu = QString::fromUtf8(message.mid(indexNom, finNom - indexNom));

            QString texteLigne = "🚗 " + nomRecu + " — " + QString::number(kmRecu) + " km";
            ui->listVehicules->addItem(texteLigne);

            QFile fichier("vehicules.txt");
            if (fichier.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream flux(&fichier);
                flux << nomRecu << ";" << kmRecu << "\n";
                fichier.close();
            }

            QByteArray confirmation;
            confirmation.append(static_cast<char>(0x00));
            confirmation.append('v');
            confirmation.append(static_cast<char>((kmRecu >> 24) & 0xFF));
            confirmation.append(static_cast<char>((kmRecu >> 16) & 0xFF));
            confirmation.append(static_cast<char>((kmRecu >> 8) & 0xFF));
            confirmation.append(static_cast<char>(kmRecu & 0xFF));
            confirmation.append(nomRecu.toUtf8());
            confirmation.append('\0');
            confirmation[0] = static_cast<char>(confirmation.size());

            if (pClient) { pClient->sendBinaryMessage(confirmation); }
        }

        // --- 4. DEMANDE DE L'HISTORIQUE DES SESSIONS ---
        if (cmd == 'H') {
            qDebug() << "[SERVEUR] Trame demande HISTORIQUE SESSIONS reçue.";
            QFile fichierS("sessions.txt");
            if (fichierS.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream fluxS(&fichierS);
                while (!fluxS.atEnd()) {
                    QString ligne = fluxS.readLine();
                    QStringList parts = ligne.split(";");
                    if (parts.size() == 5) {
                        QByteArray repS;
                        repS.append(static_cast<char>(0x00));
                        repS.append('s');
                        repS.append(parts[0].toUtf8()); repS.append('\0');
                        repS.append(parts[1].toUtf8()); repS.append('\0');
                        repS.append(parts[2].toUtf8()); repS.append('\0');
                        repS.append(parts[3].toUtf8()); repS.append('\0');
                        repS.append(parts[4].toUtf8()); repS.append('\0');
                        repS[0] = static_cast<char>(repS.size());

                        if (pClient) { pClient->sendBinaryMessage(repS); }
                    }
                }
                fichierS.close();
            }
        }

        // --- 5. AJOUT D'UNE SESSION ('P') ---
        if (cmd == 'P') {
            qDebug() << "[SERVEUR] Trame PROGRAMMATION SESSION reçue.";

            int iBorne = 2;
            int fBorne = message.indexOf('\0', iBorne);
            QString borne = QString::fromUtf8(message.mid(iBorne, fBorne - iBorne));

            int iVehicule = fBorne + 1;
            int fVehicule = message.indexOf('\0', iVehicule);
            QString vehicule = QString::fromUtf8(message.mid(iVehicule, fVehicule - iVehicule));

            int iJours = fVehicule + 1;
            int fJours = message.indexOf('\0', iJours);
            QString jours = QString::fromUtf8(message.mid(iJours, fJours - iJours));

            int iStart = fJours + 1;
            int fStart = message.indexOf('\0', iStart);
            QString start = QString::fromUtf8(message.mid(iStart, fStart - iStart));

            int iEnd = fStart + 1;
            int fEnd = message.indexOf('\0', iEnd);
            QString end = QString::fromUtf8(message.mid(iEnd, fEnd - iEnd));

            // --- AFFICHAGE SUR L'IHM DU SERVEUR (Avec Tri) ---
            QString texteLigne = "📅 " + jours + " (" + start + "-" + end + ") | " + vehicule;

            if (borne == "Borne Camille") {
                ui->listSessionsCamille->addItem(texteLigne);
            }
            if (borne == "Borne Direction") {
                ui->listSessionsDirection->addItem(texteLigne);
            }

            // --- SAUVEGARDE ---
            QFile fichierS("sessions.txt");
            if (fichierS.open(QIODevice::Append | QIODevice::Text)) {
                QTextStream fluxS(&fichierS);
                fluxS << borne << ";" << vehicule << ";" << jours << ";" << start << ";" << end << "\n";
                fichierS.close();
            }

            // --- RENVOI AU MOBILE ---
            QByteArray repS;
            repS.append(static_cast<char>(0x00));
            repS.append('s');
            repS.append(borne.toUtf8()); repS.append('\0');
            repS.append(vehicule.toUtf8()); repS.append('\0');
            repS.append(jours.toUtf8()); repS.append('\0');
            repS.append(start.toUtf8()); repS.append('\0');
            repS.append(end.toUtf8()); repS.append('\0');
            repS[0] = static_cast<char>(repS.size());

            if (pClient) { pClient->sendBinaryMessage(repS); }
        }

        // --- 6. SIMULATION RÉCEPTION ESP ('C') ---
        if (cmd == 'C') {
            // On s'assure que la trame fait bien la taille attendue (7 octets)
            bool isTrameComplete = (message.size() == 7);

            if (isTrameComplete) {
                // Extraction de chaque octet en entier non signé (quint8)
                quint8 taille = static_cast<quint8>(message[0]);
                quint8 masqueJours = static_cast<quint8>(message[2]);
                quint8 hd = static_cast<quint8>(message[3]);
                quint8 md = static_cast<quint8>(message[4]);
                quint8 hf = static_cast<quint8>(message[5]);
                quint8 mf = static_cast<quint8>(message[6]);

                // Affichage détaillé dans la console pour ton test unitaire
                qDebug() << "[SERVEUR-ESP] Trame 'C' reçue ! "
                         << "Taille:" << taille
                         << "| Jours (masque):" << masqueJours
                         << "| Début:" << hd << "h" << md
                         << "| Fin:" << hf << "h" << mf;
            }
        }
    }
}

void MainWindow::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
        qDebug() << "[SERVEUR] Client mobile déconnecté.";
    }
}
