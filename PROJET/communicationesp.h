#ifndef COMMUNICATIONESP_H
#define COMMUNICATIONESP_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QTimer>

/**
 * @file communicationesp.h
 * @brief Gestion de la communication WebSocket entre l'application mobile et le boîtier ESP32.
 * @author Coco
 * @version 1.0
 * @date Avril 2026
 */

/// @brief Port WebSocket utilisé par le boîtier ESP32.
#define ESP_PORT 5555

/**
 * @class CommunicationEsp
 * @brief Classe gérant la communication WebSocket entre l'application Qt/QML et le boîtier ESP32.
 *
 * Cette classe permet d'envoyer des trames JSON vers l'ESP32 pour :
 * - Obtenir, ajouter et supprimer des calendriers de charge
 * - Activer ou désactiver la marche forcée du relais
 *
 * Elle gère également la réception des réponses de l'ESP32 et émet des signaux
 * vers le QML pour mettre à jour l'interface utilisateur.
 * En cas de déconnexion, une tentative de reconnexion automatique est effectuée toutes les 5 secondes.
 */
class CommunicationEsp : public QObject {
    Q_OBJECT

    /// @brief Nom de la borne active, synchronisé avec le QML.
    Q_PROPERTY(QString activeBorne READ activeBorne WRITE setActiveBorne NOTIFY activeBorneChanged)

    /// @brief Nom du véhicule actif, synchronisé avec le QML.
    Q_PROPERTY(QString activeVehicule READ activeVehicule WRITE setActiveVehicule NOTIFY activeVehiculeChanged)

public:
    /**
     * @brief Constructeur de la classe CommunicationEsp.
     * @param parent Objet parent Qt (nullptr par défaut).
     */
    explicit CommunicationEsp(QObject *parent = nullptr);

    /**
     * @brief Destructeur. Ferme la connexion WebSocket et arrête le timer de reconnexion.
     */
    ~CommunicationEsp();

    /**
     * @brief Définit l'adresse IP de l'ESP32 et ouvre la connexion WebSocket.
     * @param ip Adresse IP de l'ESP32 (ex : "192.168.1.50").
     */
    Q_INVOKABLE void connexion(const QString &ip);

    /**
     * @brief Envoie une requête à l'ESP32 pour obtenir la liste des calendriers de charge.
     * Active le flag d'attente pour déclencher un clear de la liste QML à la réception.
     */
    Q_INVOKABLE void obtenirCalendrier();

    /**
     * @brief Envoie une requête à l'ESP32 pour ajouter un calendrier de charge.
     * @param jours Jours sélectionnés sous forme de texte (ex : "Lun, Mar, Mer").
     * @param start Heure de début au format "HH:MM".
     * @param end Heure de fin au format "HH:MM" (peut contenir " (lendemain)").
     */
    Q_INVOKABLE void ajouterCalendrier(const QString &jours, const QString &start, const QString &end, bool resetListe = true);
    /**
     * @brief Envoie une requête à l'ESP32 pour supprimer un calendrier de charge.
     * @param id Identifiant du calendrier à supprimer.
     */
    Q_INVOKABLE void supprimerCalendrier(int id);

    /**
     * @brief Envoie une commande de marche forcée à l'ESP32 pour contrôler le relais.
     * @param activer true pour activer la charge, false pour l'arrêter.
     */
    Q_INVOKABLE void marcheForcee(bool activer);

    /**
     * @brief Retourne le nom de la borne actuellement active.
     * @return Nom de la borne active.
     */
    QString activeBorne() const;

    /**
     * @brief Définit le nom de la borne active.
     * @param borne Nom de la borne.
     */
    void setActiveBorne(const QString &borne);

    /**
     * @brief Retourne le nom du véhicule actuellement sélectionné.
     * @return Nom du véhicule actif.
     */
    QString activeVehicule() const;

    /**
     * @brief Définit le nom du véhicule actif.
     * @param vehicule Nom du véhicule.
     */
    void setActiveVehicule(const QString &vehicule);

signals:
    /**
     * @brief Émis lorsque le statut de connexion change.
     * @param status "Connecté" ou "Déconnecté".
     */
    void connectionStatusChanged(QString status);

    /**
     * @brief Émis lorsqu'un calendrier est reçu depuis l'ESP32.
     * @param id Identifiant du calendrier.
     * @param borne Nom de la borne concernée.
     * @param vehicule Nom du véhicule concerné.
     * @param jours Jours de charge (ex : "Lun, Mar").
     * @param start Heure de début (ex : "08:00").
     * @param end Heure de fin (ex : "17:00" ou "09:00 (lendemain)").
     */
    void calendrierRecu(int id, QString borne, QString vehicule, QString jours,
                        QString start, QString end);

    /**
     * @brief Émis lorsqu'une alerte est reçue depuis l'ESP32.
     * @param type Type d'alerte (ex : "surchauffe").
     * @param message Message descriptif de l'alerte.
     */
    void alerteRecue(QString type, QString message);

    /**
     * @brief Émis pour demander au QML de vider la liste des calendriers
     *        avant de recevoir la nouvelle liste mise à jour.
     */
    void clearCalendriers();

    /// @brief Émis lorsque la borne active change.
    void activeBorneChanged();

    /// @brief Émis lorsque le véhicule actif change.
    void activeVehiculeChanged();

private slots:
    /// @brief Appelé lors de l'établissement de la connexion WebSocket.
    void onConnected();

    /// @brief Appelé lors de la déconnexion WebSocket. Lance le timer de reconnexion.
    void onDisconnected();

    /**
     * @brief Appelé à la réception d'un message WebSocket depuis l'ESP32.
     * @param message Trame JSON brute reçue.
     */
    void onTextMessageReceived(const QString &message);

    void gererChangementEtat(QAbstractSocket::SocketState etat);

    /// @brief Tente de rouvrir la connexion WebSocket vers l'ESP32.
    void tenterReconnexion();

private:
    /// @brief Socket WebSocket pour la communication avec l'ESP32.
    QWebSocket m_webSocket;
    //enum QAbstractSocket::SocketState

    /// @brief Timer déclenchant les tentatives de reconnexion toutes les 5 secondes.
    QTimer m_reconnectTimer;

    /// @brief URL WebSocket de l'ESP32 (ex : "ws://192.168.1.50:5555").
    QString m_url;

    /// @brief Nom de la borne actuellement active.
    QString m_activeBorne;

    /// @brief Nom du véhicule actuellement sélectionné.
    QString m_activeVehicule;

    /// @brief Flag indiquant qu'une nouvelle liste de calendriers est attendue.
    bool m_attenteNouveauListe;

    /**
     * @brief Convertit une chaîne de jours en masque binaire pour l'envoi à l'ESP32.
     * @param jours Jours sous forme de texte (ex : "Lun, Mar, Dim").
     * @return Masque binaire représentant les jours (bit 0 = Lun, bit 6 = Dim).
     */
    int joursVersmasque(const QString &jours);

    /**
     * @brief Extrait les heures et minutes depuis une chaîne au format "HH:MM".
     * @param heureStr Chaîne de temps (ex : "08:30").
     * @param heure Référence recevant l'heure extraite.
     * @param minute Référence recevant les minutes extraites.
     */
    void extraireHeureMinute(const QString &heureStr, int &heure, int &minute);

    /**
     * @brief Convertit un masque binaire de jours en chaîne lisible.
     * @param masque Masque binaire (bit 0 = Lun, bit 6 = Dim).
     * @return Chaîne de jours (ex : "Lun, Mar, Dim").
     */
    QString masqueVersJours(int masque);

    /**
     * @brief Formate une heure et des minutes en chaîne "HH:MM".
     * @param heure Heure (0-23).
     * @param minute Minutes (0-59).
     * @return Chaîne formatée (ex : "08:05").
     */
    QString formaterHeure(int heure, int minute);

    friend class TestCommunicationEsp;
};

#endif // COMMUNICATIONESP_H
