#ifndef ACCESBDD_H
#define ACCESBDD_H

#include <QObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtWebSockets/QWebSocket>
#include <stdexcept> // Pour std::runtime_error
#include <QString>
#include "accesbdd.h"

class accesBdd : public QObject
{
    Q_OBJECT
public:
    accesBdd(QObject *parent = nullptr);
    bool connecter();

    // Méthodes avec gestion d'exceptions
    //void insererMesure(float puissance);
    void ajouterVehicule(const QString &nom, int km);
    void envoyerListeVehicules(QWebSocket *pClient);
    void envoyerListeBornes(QWebSocket *pClient);
    void supprimerVehicule(int id);
    void modifierKilometrage(int id, int km);

    void ajouterEvenement(bool type_alerte, const QString &message_erreur, int id_borne);

private:
    QSqlDatabase bdd;
};

#endif // ACCESBDD_H
