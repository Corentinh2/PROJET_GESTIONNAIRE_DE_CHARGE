#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QList>
#include <QSqlDatabase>

class Serveur : public QObject
{
    Q_OBJECT

public:
    explicit Serveur(quint16 port, QObject *parent = nullptr);
    ~Serveur();

private slots:
    void onNewConnection();
    void onTextMessageReceived(const QString &message);
    void onDisconnected();

private:
    QWebSocketServer *socketServeur;
    QList<QWebSocket *> listeClients;
    void insererEnBase(float u, float i, float e);
};

#endif // SERVEUR_H
