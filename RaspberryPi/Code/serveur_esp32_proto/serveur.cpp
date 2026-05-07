#include "serveur.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

Serveur::Serveur(quint16 port, QObject *parent) : QObject(parent)
{
    socketServeur = new QWebSocketServer("Serveur ESP32 Proto", QWebSocketServer::NonSecureMode, this);
    connecterAccesBDD();

    if (socketServeur->listen(QHostAddress::Any, port)) {
        qDebug() << "SERVEUR VERSION 2.0 - TEST 11h42" << port;
        connect(socketServeur, &QWebSocketServer::newConnection, this, &Serveur::onNewConnection);
    }
}

Serveur::~Serveur()
{
    socketServeur->close();
    qDeleteAll(listeClients.begin(), listeClients.end());
}

bool Serveur::connecterAccesBDD()
{
    bool resultat = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("gestionnaire_de_charge");
    db.setUserName("charge");
    db.setPassword("ciel");
    db.setPort(3306);

    if (!db.open()) {
        qDebug() << "Erreur AccesBDD :" << db.lastError().text();
        qDebug() << "Erreur driver :" << db.lastError().driverText();
    }

    if (db.isOpen()) {
        qDebug() << "Connecté à GestionnaireDeCharge !";
        resultat = true;
    }

    return resultat;
}

void Serveur::onNewConnection()
{
    QWebSocket *client = socketServeur->nextPendingConnection();
    listeClients.append(client);

    qDebug() << "[CLIENT CONNECTÉ] -" << client->peerAddress().toString();

    connect(client, &QWebSocket::textMessageReceived, this, &Serveur::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected, this, &Serveur::onDisconnected);
}

void Serveur::onTextMessageReceived(const QString &message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (pClient && !doc.isNull()) {
        qDebug() << "Trame reçue :" << message;
        QJsonObject obj = doc.object();
        QSqlQuery query;

        // --- CAS 1 : Puissance (ESP32) ---
        if (obj.contains("puissance")) {
            insererEnBase(obj["puissance"].toDouble());
        }

        // --- CAS 2 : Actions (Mobile) ---
        if (obj.contains("action")) {
            QString action = obj["action"].toString();

            // Action : obtenirStation
            if (action == "obtenirStation") {
                maBdd.envoyerListeBornes(pClient);
            }
            // Action : obtenirVehicule
            if (action == "obtenirVehicule") {
                maBdd.envoyerListeVehicules(pClient);
            }
            // Action : ajouterVehicule
            if (action == "ajouterVehicule") {
                maBdd.ajouterVehicule(obj["name"].toString(), obj["km"].toInt());
                maBdd.envoyerListeVehicules(pClient);
            }
            // Action : supprimerVehicule
            if (action == "supprimerVehicule") {
                maBdd.supprimerVehicule(obj["id"].toInt());
                maBdd.envoyerListeVehicules(pClient);
            }
            // Action : modifierKilometrage
            if (action == "modifierKilometrage") {
                maBdd.modifierKilometrage(obj["id"].toInt(), obj["km"].toInt());
                maBdd.envoyerListeVehicules(pClient);
            }
            // Action : envoyerAlerte
            if (action == "alerte") {
                int typeRecu = obj["type"].toInt(); // L'ESP32 envoie 0 ou 1
                QString msg  = obj["message"].toString();
                int idBorne  = obj["idBorne"].toInt();

                // On traite le courant (0)
                if (typeRecu == 0) {
                    maBdd.ajouterEvenement(false, msg, idBorne); // false enverra 0 en BDD
                    qDebug() << "Alerte COURANT enregistrée.";
                }

                // On traite la température (1)
                if (typeRecu == 1) {
                    maBdd.ajouterEvenement(true, msg, idBorne); // true enverra 1 en BDD
                    qDebug() << "Alerte TEMPÉRATURE enregistrée.";
                }

                // --- Notification commune au mobile ---
                QJsonObject notif;
                notif["action"] = "alerteRecue";
                notif["type"] = typeRecu;
                notif["message"] = msg;

                for (QWebSocket *client : listeClients) {
                    client->sendTextMessage(QJsonDocument(notif).toJson(QJsonDocument::Compact));
                }
            }

        }
    }
}

void Serveur::insererEnBase(float puissance)
{
    QSqlQuery query;
    query.prepare("INSERT INTO MESURE (horodatage, puissance, id_charge) VALUES (NOW(), :p, :id)");
    query.bindValue(":p", puissance);
    query.bindValue(":id", 4);

    if (query.exec()) {
        qDebug() << "Mesure insérée ! Puissance:" << puissance << "W";
    }

    if (!query.isActive()) {
        qDebug() << "Erreur insertion :" << query.lastError().text();
    }
}

void Serveur::onDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        listeClients.removeAll(client);
        client->deleteLater();
        qDebug() << "[CLIENT DÉCONNECTÉ]" << client->peerAddress().toString();
    }
}

