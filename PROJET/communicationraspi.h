#ifndef COMMUNICATIONRASPI_H
#define COMMUNICATIONRASPI_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QTimer>

/**
 * @file communicationraspi.h
 * @brief Gestion de la communication WebSocket entre l'application mobile et le Raspberry Pi.
 * @author Coco
 * @version 1.0
 * @date Avril 2026
 */

/// @brief Adresse IP du Raspberry Pi sur le réseau local.
#define RASPI_IP   "192.168.2.90"

/// @brief Port WebSocket utilisé par le serveur Raspberry Pi.
#define RASPI_PORT 8080

/**
 * @class CommunicationRaspi
 * @brief Classe gérant la communication WebSocket entre l'application Qt/QML et le Raspberry Pi.
 *
 * Cette classe permet d'envoyer des requêtes JSON vers le Raspberry Pi pour :
 * - Obtenir la liste des bornes de charge disponibles
 * - Obtenir, ajouter, modifier et supprimer des véhicules
 *
 * Elle gère également la réception des réponses et émet des signaux vers le QML
 * pour mettre à jour l'interface utilisateur.
 * En cas de déconnexion, une tentative de reconnexion automatique est effectuée toutes les 5 secondes.
 */
class CommunicationRaspi : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructeur de la classe CommunicationRaspi.
     *
     * Initialise la connexion WebSocket vers le Raspberry Pi et configure
     * le timer de reconnexion automatique. Appelle obtenirStation() dès
     * que la connexion est établie.
     *
     * @param parent Objet parent Qt (nullptr par défaut).
     */
    explicit CommunicationRaspi(QObject *parent = nullptr);

    /**
     * @brief Destructeur. Arrête le timer de reconnexion et ferme la connexion WebSocket.
     */
    ~CommunicationRaspi();

    /**
     * @brief Envoie une requête au Raspberry Pi pour obtenir la liste des bornes.
     *
     * Trame envoyée : {"action":"obtenirStation"}
     */
    Q_INVOKABLE void obtenirStation();

    /**
     * @brief Envoie une requête au Raspberry Pi pour obtenir la liste des véhicules.
     *
     * Trame envoyée : {"action":"obtenirVehicule"}
     */
    Q_INVOKABLE void obtenirVehicule();

    /**
     * @brief Envoie une requête au Raspberry Pi pour ajouter un véhicule.
     *
     * Trame envoyée : {"action":"ajouterVehicule","name":name,"km":km}
     *
     * @param name Nom/modèle du véhicule (ex : "Renault Zoé").
     * @param km Kilométrage actuel du véhicule.
     */
    Q_INVOKABLE void ajouterVehicule(const QString &name, const QString &km);

    /**
     * @brief Envoie une requête au Raspberry Pi pour modifier le kilométrage d'un véhicule.
     *
     * Trame envoyée : {"action":"modifierKilometrage","id":id,"km":km}
     *
     * @param id Identifiant du véhicule à modifier.
     * @param km Nouveau kilométrage du véhicule.
     */
    Q_INVOKABLE void modifierKilometrage(int id, int km);

    /**
     * @brief Envoie une requête au Raspberry Pi pour supprimer un véhicule.
     *
     * Trame envoyée : {"action":"supprimerVehicule","id":id}
     *
     * @param id Identifiant du véhicule à supprimer.
     */
    Q_INVOKABLE void supprimerVehicule(int id);

signals:
    /**
     * @brief Émis lorsque le statut de connexion au Raspberry Pi change.
     * @param status "Connecté" ou "Déconnecté".
     */
    void connectionStatusChanged(QString status);

    /**
     * @brief Émis lorsqu'une borne est reçue depuis le Raspberry Pi.
     * @param id Identifiant de la borne.
     * @param name Nom de la borne.
     * @param kwh Puissance de la borne en watts.
     * @param status Statut de la borne ("Disponible", "En Charge", "Programmé").
     * @param ip Adresse IP de l'ESP32 associé à la borne.
     */
    void stationRecue(int id, QString name, QString kwh, QString status, QString ip);

    /**
     * @brief Émis lorsqu'un véhicule est reçu depuis le Raspberry Pi.
     * @param id Identifiant du véhicule.
     * @param name Nom/modèle du véhicule.
     * @param km Kilométrage actuel du véhicule.
     */
    void vehiculeRecu(int id, QString name, QString km);

private slots:
    /**
     * @brief Appelé lors de l'établissement de la connexion WebSocket.
     *
     * Arrête le timer de reconnexion, émet le signal de statut
     * et appelle obtenirStation() pour récupérer la liste des bornes.
     */
    void onConnected();

    /**
     * @brief Appelé lors de la déconnexion WebSocket.
     *
     * Émet le signal de changement de statut et démarre le timer
     * de reconnexion automatique.
     */
    void onDisconnected();

    /**
     * @brief Gère les changements d'état du socket WebSocket.
     * Démarre le timer de reconnexion automatique si le socket passe à l'état
     * déconnecté, et l'arrête si le socket passe à l'état connecté.
     * @param etat Nouvel état du socket WebSocket.
     */
    void gererChangementEtat(QAbstractSocket::SocketState etat);

    /**
     * @brief Appelé à la réception d'un message WebSocket depuis le Raspberry Pi.
     * @param message Trame JSON brute reçue.
     */
    void onTextMessageReceived(const QString &message);

    /**
     * @brief Tente de rouvrir la connexion WebSocket vers le Raspberry Pi.
     * Ne fait rien si le socket n'est pas dans l'état déconnecté,
     * évitant ainsi les tentatives de connexion multiples.
     */
    void tenterReconnexion();

private:
    /// @brief Socket WebSocket pour la communication avec le Raspberry Pi.
    QWebSocket m_webSocket;

    /// @brief Timer déclenchant les tentatives de reconnexion toutes les 5 secondes.
    QTimer m_reconnectTimer;

    /// @brief URL WebSocket du Raspberry Pi (ex : "ws://192.168.2.90:8080").
    QString m_url;
};

#endif // COMMUNICATIONRASPI_H
