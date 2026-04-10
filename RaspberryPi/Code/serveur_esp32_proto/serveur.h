#ifndef SERVEUR_H
#define SERVEUR_H
#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QJsonObject>
#include "accesbdd.h"

class Serveur : public QObject
{
    Q_OBJECT
public:
    explicit Serveur(quint16 port, QObject *parent = nullptr);
    ~Serveur();
    bool connecterAccesBDD();

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();


private:
    QWebSocketServer *socketServeur;
    QList<QWebSocket *> listeClients;
    accesBdd maBdd;
    void insererEnBase(float puissance);


};

#endif // SERVEUR_H
