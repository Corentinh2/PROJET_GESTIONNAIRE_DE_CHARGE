#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "communicationraspi.h"
#include "communicationesp.h"
#include "communicationtemp.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // 1. On crée l'objet
    //CommunicationTemp communication;

    CommunicationRaspi commRaspi;
    CommunicationEsp   commEsp;

    // 2. ON LE DONNE AU QML
    // Le premier paramètre "commTemp" DOIT être identique
    // au nom utilisé dans ton fichier Main.qml
    //engine.rootContext()->setContextProperty("commTemp", &communication);

    engine.rootContext()->setContextProperty("commRaspi", &commRaspi);
    engine.rootContext()->setContextProperty("commEsp",   &commEsp);

    // 3. Chargement du module
    engine.loadFromModule("PROJET", "Main");

    int result = app.exec();
    return result;
}
