#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext> // <--- Indispensable pour setContextProperty
#include "communicationtemp.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // 1. On crée l'objet
    CommunicationTemp communication;

    // 2. ON LE DONNE AU QML
    // Le premier paramètre "commTemp" DOIT être identique
    // au nom utilisé dans ton fichier Main.qml
    engine.rootContext()->setContextProperty("commTemp", &communication);

    // 3. Chargement du module
    engine.loadFromModule("PROJET", "Main");

    int result = app.exec();
    return result;
}
