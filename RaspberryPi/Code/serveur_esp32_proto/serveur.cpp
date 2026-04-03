#include "serveur.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

Serveur::Serveur(quint16 port, QObject *parent) : QObject(parent)
{
    socketServeur = new QWebSocketServer("Serveur ESP32 Proto", QWebSocketServer::NonSecureMode, this);

    if (socketServeur->listen(QHostAddress::Any, port)) {
        qDebug() << "SERVEUR VERSION 2.0 - TEST 15h42" << port;
        connect(socketServeur, &QWebSocketServer::newConnection, this, &Serveur::onNewConnection);
    }
}

Serveur::~Serveur()
{
    socketServeur->close();
    qDeleteAll(listeClients.begin(), listeClients.end());
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
    // On reçoit la trame de l'ESP32 : "V;I;Wh"
    QStringList liste = message.split(';');

    if (liste.size() >= 3) {
        float U = liste[0].toFloat();
        float I = liste[1].toFloat();
        float Wh = liste[2].toFloat();

        qDebug() << "Données reçues -> U:" << U << "V, I:" << I << "A, E:" << Wh << "Wh";

        insererEnBase(U, I, Wh);
    }
}

void Serveur::insererEnBase(float u, float i, float e)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("ma_base_borne"); // À adapter
    db.setUserName("root");              // À adapter
    db.setPassword("password");          // À adapter

    if (db.open()) {
        QSqlQuery query;
        query.prepare("INSERT INTO mesures (date_heure, tension, courant, energie) VALUES (:date, :u, :i, :e)");
        query.bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        query.bindValue(":u", u);
        query.bindValue(":i", i);
        query.bindValue(":e", e);

        if(!query.exec()) qDebug() << "Erreur SQL :" << query.lastError().text();
        db.close();
    }
}

void Serveur::onDisconnected()
{
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        listeClients.removeAll(client);
        client->deleteLater();
        qDebug() << "[CLIENT DÉCONNECTÉ]";
    }
}
