#ifndef COMMUNICATIONTEMP_H
#define COMMUNICATIONTEMP_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>

class CommunicationTemp : public QObject {
    Q_OBJECT

public:
    explicit CommunicationTemp(QObject *parent = nullptr);
    ~CommunicationTemp();

    Q_INVOKABLE void requestStationsList();
    Q_INVOKABLE void requestVehiclesList();
    Q_INVOKABLE void addVehicleToDb(const QString &name, const QString &km);
    Q_INVOKABLE void requestSessionsHistory();

    // NOUVEAU : Fonction appelée par le QML pour envoyer les deux trames (Raspi + ESP)
    Q_INVOKABLE void programmerCharge(const QString &borne, const QString &vehicule, const QString &jours, const QString &start, const QString &end);

signals:
    void connectionStatusChanged(QString status);
    // 100% SANS JSON : On envoie les variables directement !
    void stationReceived(QString name, QString kwh, QString status);
    void vehicleReceived(QString name, QString km);

    // NOUVEAU : Signal émis quand on reçoit une trame 's' du serveur
    void sessionReceived(QString borne, QString vehicule, QString jours, QString start, QString end);

private slots:
    void onConnected();
    void onBinaryMessageReceived(QByteArray message); // Réception Binaire !

private:
    QWebSocket m_webSocket;
};

#endif // COMMUNICATIONTEMP_H
