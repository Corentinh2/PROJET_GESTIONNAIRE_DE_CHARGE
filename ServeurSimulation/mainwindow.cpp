#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

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

    // --- CHARGEMENT DES SESSIONS AU DÉMARRAGE ---
    QFile fichierSessions("sessions.txt");
    if (fichierSessions.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream fluxS(&fichierSessions);
        while (!fluxS.atEnd()) {
            QString ligne = fluxS.readLine();
            QStringList parts = ligne.split(";");
            if (parts.size() == 5) {
                QString nomBorne = parts[0];
                QString affichage = "📅 " + parts[2] + " (" + parts[3] + "-" + parts[4] + ") | " + parts[1];
                if (nomBorne == "Borne Camille") {
                    ui->listSessionsCamille->addItem(affichage);
                }
            }
        }
        fichierSessions.close();
    }

    // --- INITIALISATION DU SERVEUR ---
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Serveur Simulation"),
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

    connect(pSocket, &QWebSocket::textMessageReceived, this, &MainWindow::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);

    m_clients << pSocket;
    qDebug() << "[SERVEUR] Un client vient de se connecter !";
}

void MainWindow::processTextMessage(const QString &message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) {
        qDebug() << "[SERVEUR] Message invalide :" << message;
        return;
    }

    QJsonObject json = doc.object();
    QString action = json["action"].toString();

    // ================================================================
    // 0. IDENTIFICATION DU CLIENT
    // ================================================================
    if (action == "identify") {
        QString role = json["client"].toString();
        m_clientRoles[pClient] = role;
        qDebug() << "[SERVEUR] Client identifié comme :" << role;
        return;
    }

    // Vérification du rôle du client
    QString clientRole = m_clientRoles.value(pClient, "inconnu");

    // ================================================================
    // 1. DEMANDE DES BORNES (Raspi uniquement)
    // ================================================================
    if (action == "getStations" && clientRole == "raspi") {
        qDebug() << "[SERVEUR] Requête getStations reçue (client raspi).";

        QJsonObject borne1;
        borne1["action"] = "station";
        borne1["name"]   = "Borne Camille";
        borne1["kwh"]    = "22 kW - Parking Nord";
        borne1["status"] = "Disponible";
        if (pClient) {
            pClient->sendTextMessage(QJsonDocument(borne1).toJson(QJsonDocument::Compact));
        }

        QJsonObject borne2;
        borne2["action"] = "station";
        borne2["name"]   = "Borne Direction";
        borne2["kwh"]    = "50 kW - Parking Sud";
        borne2["status"] = "Disponible";
        if (pClient) {
            pClient->sendTextMessage(QJsonDocument(borne2).toJson(QJsonDocument::Compact));
        }

        qDebug() << "[SERVEUR] Bornes envoyées.";
    }

    // ================================================================
    // 2. DEMANDE DES VÉHICULES (Raspi uniquement)
    // ================================================================
    if (action == "getVehicles" && clientRole == "raspi") {
        qDebug() << "[SERVEUR] Requête getVehicles reçue (client raspi).";

        QFile fichier("vehicules.txt");
        if (fichier.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream flux(&fichier);
            while (!flux.atEnd()) {
                QString ligne = flux.readLine();
                QStringList parts = ligne.split(";");
                if (parts.size() == 2) {
                    QJsonObject vehicule;
                    vehicule["action"] = "vehicle";
                    vehicule["name"]   = parts[0];
                    vehicule["km"]     = parts[1].toInt();

                    if (pClient) {
                        pClient->sendTextMessage(QJsonDocument(vehicule).toJson(QJsonDocument::Compact));
                    }
                }
            }
            fichier.close();
        }

        qDebug() << "[SERVEUR] Véhicules envoyés.";
    }

    // ================================================================
    // 3. AJOUT D'UN VÉHICULE (Raspi uniquement)
    // ================================================================
    if (action == "addVehicle" && clientRole == "raspi") {
        qDebug() << "[SERVEUR] Requête addVehicle reçue (client raspi).";

        QString nom = json["name"].toString();
        int km      = json["km"].toInt();

        QString texteLigne = "🚗 " + nom + " — " + QString::number(km) + " km";
        ui->listVehicules->addItem(texteLigne);

        QFile fichier("vehicules.txt");
        if (fichier.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream flux(&fichier);
            flux << nom << ";" << km << "\n";
            fichier.close();
        }

        QJsonObject confirmation;
        confirmation["action"] = "vehicle";
        confirmation["name"]   = nom;
        confirmation["km"]     = km;

        if (pClient) {
            pClient->sendTextMessage(QJsonDocument(confirmation).toJson(QJsonDocument::Compact));
        }

        qDebug() << "[SERVEUR] Véhicule ajouté :" << nom;
    }

    // ================================================================
    // 4. DEMANDE DE L'HISTORIQUE DES SESSIONS (ESP uniquement)
    // ================================================================
    if (action == "getSessions" && clientRole == "esp") {
        qDebug() << "[SERVEUR] Requête getSessions reçue (client esp).";

        QFile fichierS("sessions.txt");
        if (fichierS.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream fluxS(&fichierS);
            while (!fluxS.atEnd()) {
                QString ligne = fluxS.readLine();
                QStringList parts = ligne.split(";");
                if (parts.size() == 5) {
                    QJsonObject session;
                    session["action"]   = "session";
                    session["borne"]    = parts[0];
                    session["vehicule"] = parts[1];
                    session["jours"]    = parts[2];
                    session["start"]    = parts[3];
                    session["end"]      = parts[4];

                    if (pClient) {
                        pClient->sendTextMessage(QJsonDocument(session).toJson(QJsonDocument::Compact));
                    }
                }
            }
            fichierS.close();
        }

        qDebug() << "[SERVEUR] Sessions envoyées.";
    }

    // ================================================================
    // 5. PROGRAMMATION D'UNE SESSION (ESP uniquement)
    // ================================================================
    if (action == "programCharge" && clientRole == "esp") {
        qDebug() << "[SERVEUR] Requête programCharge reçue (client esp).";

        QString borne    = json["borne"].toString();
        QString vehicule = json["vehicule"].toString();
        QString jours    = json["jours"].toString();
        QString start    = json["start"].toString();
        QString end      = json["end"].toString();

        QString texteLigne = "📅 " + jours + " (" + start + "-" + end + ") | " + vehicule;
        if (borne == "Borne Camille") {
            ui->listSessionsCamille->addItem(texteLigne);
        }
        if (borne == "Borne Direction") {
            ui->listSessionsDirection->addItem(texteLigne);
        }

        QFile fichierS("sessions.txt");
        if (fichierS.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream fluxS(&fichierS);
            fluxS << borne << ";" << vehicule << ";" << jours << ";" << start << ";" << end << "\n";
            fichierS.close();
        }

        QJsonObject confirmation;
        confirmation["action"]   = "session";
        confirmation["borne"]    = borne;
        confirmation["vehicule"] = vehicule;
        confirmation["jours"]    = jours;
        confirmation["start"]    = start;
        confirmation["end"]      = end;

        if (pClient) {
            pClient->sendTextMessage(QJsonDocument(confirmation).toJson(QJsonDocument::Compact));
        }

        qDebug() << "[SERVEUR] Session programmée :" << borne << vehicule;
    }

    // ================================================================
    // 6. SUPPRESSION D'UNE SESSION (ESP uniquement)
    // ================================================================
    if (action == "deleteCharge" && clientRole == "esp") {
        qDebug() << "[SERVEUR] Requête deleteCharge reçue (client esp).";

        QString borne    = json["borne"].toString();
        QString vehicule = json["vehicule"].toString();
        QString jours    = json["jours"].toString();
        QString start    = json["start"].toString();
        QString end      = json["end"].toString();

        QStringList lignesRestantes;
        QFile fichierLecture("sessions.txt");
        if (fichierLecture.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream flux(&fichierLecture);
            while (!flux.atEnd()) {
                QString ligne = flux.readLine();
                QStringList parts = ligne.split(";");
                if (parts.size() == 5) {
                    bool correspond = false;
                    if (parts[0] == borne) {
                        if (parts[1] == vehicule) {
                            if (parts[2] == jours) {
                                if (parts[3] == start) {
                                    if (parts[4] == end) {
                                        correspond = true;
                                    }
                                }
                            }
                        }
                    }
                    if (!correspond) {
                        lignesRestantes.append(ligne);
                    }
                }
            }
            fichierLecture.close();
        }

        QFile fichierEcriture("sessions.txt");
        if (fichierEcriture.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QTextStream flux(&fichierEcriture);
            for (int i = 0; i < lignesRestantes.size(); i++) {
                flux << lignesRestantes[i] << "\n";
            }
            fichierEcriture.close();
        }

        QString texteLigne = "📅 " + jours + " (" + start + "-" + end + ") | " + vehicule;
        if (borne == "Borne Camille") {
            QList<QListWidgetItem*> items = ui->listSessionsCamille->findItems(texteLigne, Qt::MatchExactly);
            for (int i = 0; i < items.size(); i++) {
                delete ui->listSessionsCamille->takeItem(ui->listSessionsCamille->row(items[i]));
            }
        }

        qDebug() << "[SERVEUR] Session supprimée :" << borne << vehicule;
    }

    // ================================================================
    // 7. MARCHE FORCÉE (ESP uniquement)
    // ================================================================
    if (action == "forceCharge" && clientRole == "esp") {
        qDebug() << "[SERVEUR-ESP] Marche forcée reçue !";
        qDebug() << "  Borne   :" << json["borne"].toString();
        qDebug() << "  Activer :" << json["activer"].toBool();
    }
}

void MainWindow::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clientRoles.remove(pClient);
        m_clients.removeAll(pClient);
        pClient->deleteLater();
        qDebug() << "[SERVEUR] Client déconnecté.";
    }
}
