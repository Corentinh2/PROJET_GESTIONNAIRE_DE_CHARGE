#ifndef COMMUNICATIONRASPI_H
#define COMMUNICATIONRASPI_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>

// ============================================================
// CONFIGURATION SERVEUR RASPBERRY PI
// Modifier ces valeurs pour pointer vers le vrai Raspberry Pi
// ============================================================
#define RASPI_IP   "172.18.58.97"
#define RASPI_PORT 1234

class CommunicationRaspi : public QObject {
    Q_OBJECT

public:
    explicit CommunicationRaspi(QObject *parent = nullptr);
    ~CommunicationRaspi();

    // --- Requêtes de données (QML → Raspi) ---
    Q_INVOKABLE void obtenirStation();
    Q_INVOKABLE void obtenirVehicule();

    // --- Actions (QML → Raspi) ---
    Q_INVOKABLE void ajouterVehicule(const QString &name, const QString &km);
    Q_INVOKABLE void supprimerVehicule(int id);

signals:
    void connectionStatusChanged(QString status);

    // --- Réception de données (Raspi → QML) ---
    void stationRecue(int id, QString name, QString kwh, QString status);
    void vehiculeRecu(int id, QString name, QString km);

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket m_webSocket;
};

#endif // COMMUNICATIONRASPI_H
