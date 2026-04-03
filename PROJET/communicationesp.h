#ifndef COMMUNICATIONESP_H
#define COMMUNICATIONESP_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>

// ============================================================
// CONFIGURATION SERVEUR ESP32
// Modifier ces valeurs pour pointer vers le vrai ESP32
// ============================================================
#define ESP_IP   "172.18.58.97"
#define ESP_PORT 1234

class CommunicationEsp : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString activeBorne READ activeBorne WRITE setActiveBorne NOTIFY activeBorneChanged)
    Q_PROPERTY(QString activeVehicule READ activeVehicule WRITE setActiveVehicule NOTIFY activeVehiculeChanged)

public:
    explicit CommunicationEsp(QObject *parent = nullptr);
    ~CommunicationEsp();

    // --- Calendriers de charge (QML → ESP32) ---
    Q_INVOKABLE void obtenirCalendrier();
    Q_INVOKABLE void ajouterCalendrier(const QString &jours, const QString &start, const QString &end);
    Q_INVOKABLE void supprimerCalendrier(int id);

    // --- Commande de pilotage (QML → ESP32) ---
    Q_INVOKABLE void marcheForcee(bool activer);

    // --- Getters/Setters ---
    QString activeBorne() const;
    void setActiveBorne(const QString &borne);
    QString activeVehicule() const;
    void setActiveVehicule(const QString &vehicule);

signals:
    void connectionStatusChanged(QString status);

    // --- Réception de données (ESP32 → QML) ---
    void calendrierRecu(int id, QString borne, QString vehicule, QString jours,
                        QString start, QString end);
    void alerteRecue(QString type, QString message);

    // Signal pour demander au QML de vider la liste avant la nouvelle
    void clearCalendriers();

    void activeBorneChanged();
    void activeVehiculeChanged();

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket m_webSocket;

    QString m_activeBorne;
    QString m_activeVehicule;

    // Flag : le prochain calendrier reçu déclenche un clear
    bool m_attenteNouveauListe;

    // Utilitaires : texte → compact (envoi)
    int joursVersmasque(const QString &jours);
    void extraireHeureMinute(const QString &heureStr, int &heure, int &minute);

    // Utilitaires : compact → texte (réception)
    QString masqueVersJours(int masque);
    QString formaterHeure(int heure, int minute);
};

#endif // COMMUNICATIONESP_H
