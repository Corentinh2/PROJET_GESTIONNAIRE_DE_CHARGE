#include <QCoreApplication>

#include <QCoreApplication>
#include "serveur.h"

int main(int argc, char *argv[])
{
    // QCoreApplication est utilisé pour les projets Console (sans IHM)
    QCoreApplication a(argc, argv);

    // On instancie ton serveur sur le port 8080
    // C'est ici que l'écoute réseau commence
    Serveur monServeur(8080);

    qDebug() << "VERSION FINALE TEST";

    // a.exec() lance la boucle d'événements de Qt.
    // Sans cette ligne, les signaux (onNewConnection, etc.) ne fonctionneraient pas.
    return a.exec();
}
