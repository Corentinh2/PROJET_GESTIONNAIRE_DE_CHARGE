#include "accesbdd.h"



accesBdd::accesBdd(QObject *parent)
{

}

bool accesBdd::connecter()
{
    bool resultat = false;
    try {
        bdd = QSqlDatabase::addDatabase("QMYSQL");
        bdd.setHostName("localhost");
        bdd.setDatabaseName("gestionnaire_de_charge");
        bdd.setUserName("charge");
        bdd.setPassword("ciel");
        bdd.setPort(3306);

        if (!bdd.open()) {
            throw std::runtime_error(bdd.lastError().text().toStdString());
        }

        qDebug() << "Connecté à la base de données !";
        resultat = true;
    }
    catch (const std::exception& e) {
        qDebug() << "ERREUR FATALE BDD :" << e.what();
    }

    return resultat;
}

/*void accesBdd::insererMesure(float puissance)
{
    try {
        QSqlQuery query;
        query.prepare("INSERT INTO MESURE (horodatage, puissance, id_charge) VALUES (NOW(), :p, :id)");
        query.bindValue(":p", puissance);
        query.bindValue(":id", 4);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }
        qDebug() << "Mesure insérée :" << puissance << "W";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Insertion :" << e.what();
  */

void accesBdd::ajouterVehicule(const QString &nom, int km)
{
    try {
        QSqlQuery query;
        query.prepare("INSERT INTO VEHICULE (modele, kilometres) VALUES (:nom, :km)");
        query.bindValue(":nom", nom);
        query.bindValue(":km", km);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }
        qDebug() << "Véhicule" << nom << "ajouté avec succès.";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Véhicule :" << e.what();
    }
}

void accesBdd::envoyerListeVehicules(QWebSocket *pClient)
{
    try {
        if (!pClient) {
            throw std::runtime_error("Client non valide");
        }

        QSqlQuery query;
        if (!query.exec("SELECT id_vehicule, modele, kilometres FROM VEHICULE")) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        while (query.next()) {
            QJsonObject reponse;
            reponse["action"] = "vehicule";
            reponse["id"]     = query.value("id_vehicule").toInt();
            reponse["name"]   = query.value("modele").toString();
            reponse["km"]     = query.value("kilometres").toInt();
            pClient->sendTextMessage(QJsonDocument(reponse).toJson(QJsonDocument::Compact));
        }
        qDebug() << "Succès : Liste des véhicules envoyée au client.";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Liste Véhicules :" << e.what();
    }
}

void accesBdd::envoyerListeBornes(QWebSocket *pClient)
{
    try {
        if (!pClient) {
            throw std::runtime_error("Client non valide");
        }

        QSqlQuery query;
        if (!query.exec("SELECT id_borne, nom_borne, puissance, emplacement, ip_usine FROM BORNE")) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        while (query.next()) {
            QJsonObject reponse;
            reponse["action"] = "station";
            reponse["id"]     = query.value("id_borne").toInt();
            reponse["name"]   = query.value("nom_borne").toString();
            reponse["kwh"]    = query.value("puissance").toString();
            reponse["emplacement"]    = query.value("emplacement").toString();
            reponse["ip"]    = query.value("ip_usine").toString();
            pClient->sendTextMessage(QJsonDocument(reponse).toJson(QJsonDocument::Compact));
        }
        qDebug() << "Succès : Liste des bornes envoyée au client.";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Liste Bornes :" << e.what();
    }
}

void accesBdd::supprimerVehicule(int id)
{
    try {
        QSqlQuery query;
        query.prepare("DELETE FROM VEHICULE WHERE id_vehicule = :id");
        query.bindValue(":id", id);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        qDebug() << "Véhicule ID" << id << "supprimé avec succès.";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Suppression Véhicule :" << e.what();
    }
}

void accesBdd::modifierKilometrage(int id, int km)
{
    try {
        QSqlQuery query;
        query.prepare("UPDATE VEHICULE SET kilometres = :km WHERE id_vehicule = :id");
        query.bindValue(":km", km);
        query.bindValue(":id", id);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        }

        qDebug() << "Kilométrage mis à jour pour l'ID" << id << ":" << km << "km";
    }
    catch (const std::exception& e) {
        qDebug() << "Exception Modification KM :" << e.what();
    }
}

void accesBdd::ajouterEvenement(bool type_alerte, const QString &message_erreur, int id_borne)
{
    try {
        QSqlQuery query;
        query.prepare("INSERT INTO EVENEMENT (type_alerte, message_erreur, id_borne) "
                      "VALUES (:type, :msg, :borne)");

        // typeAlerte sera envoyé comme 0 (courant) ou 1 (température)
        query.bindValue(":type", type_alerte ? 1 : 0);
        query.bindValue(":msg", message_erreur);
        query.bindValue(":borne", id_borne);

        if (!query.exec()) {
            throw std::runtime_error(query.lastError().text().toStdString());
        } else {

            // Pour le débug dans ta console Raspi
            QString label = (type_alerte == 1) ? "TEMPÉRATURE (1)" : "COURANT (0)";
            qDebug() << "Succès : Événement enregistré -> Type:" << label;

        }
    }
    catch (const std::exception& e) {
        qDebug() << "Exception BDD Evenement :" << e.what();
    }
}
