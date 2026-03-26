import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

// Importation de la logique JavaScript
import "logique.js" as Logique

ApplicationWindow {
    id: window

    // --- FORMAT ET APPARENCE ---
    width: 360
    height: 720
    visible: true
    title: "Gestionnaire de charge"
    color: "#1E1E1E" // Couleur de la "coque" du téléphone
    font.family: "Segoe UI"

    // --- PROPRIÉTÉS GLOBALES (Accessibles par toutes les pages) ---
    property string activeStation: ""      // Borne choisie
    property string selectedVehicle: ""    // Véhicule pour la session
    property bool isBookingFlow: false     // État du tunnel de réservation

    // Paramètres utilisateur
    property double userCostPerKwh: 0.16
    property int userMaxPower: 12

    // Variables de session (Temps réel)
    property int sessionDurationSeconds: 0
    property double sessionEnergyKwh: 0.0
    property double sessionCostEuro: 0.0

    // --- MODÈLES DE DONNÉES (Tes BDD en mémoire) ---
    ListModel { id: notificationModel }
    ListModel { id: sessionsModel }
    ListModel { id: vehiclesModel }
    ListModel { id: stationsModelSource }

    // --- MOTEUR DE SIMULATION DE CHARGE ---
    Timer {
        id: globalChargingTimer
        interval: 1000
        repeat: true
        // On tourne si une borne est sélectionnée et qu'elle est en état "En Charge"
        running: (window.activeStation !== "") && (Logique.getSelectedStationStatus() === "En Charge")

        onTriggered: {
            window.sessionDurationSeconds += 1;
            // Calcul : Puissance (kW) / 3600 = Énergie ajoutée en 1 seconde
            var energyAdded = window.userMaxPower / 3600.0;
            window.sessionEnergyKwh += energyAdded;
            window.sessionCostEuro = window.sessionEnergyKwh * window.userCostPerKwh;
        }
    }

    // --- STRUCTURE VISUELLE (Le téléphone) ---
    Rectangle {
        id: appFrame
        anchors.fill: parent
        anchors.margins: 2
        radius: 30
        color: "#F4F6F9" // Fond de l'écran du téléphone
        border.color: "#000000"
        border.width: 5
        clip: true

        StackView {
            id: stackView
            anchors.fill: parent
            anchors.topMargin: 15
            anchors.bottomMargin: 10
            anchors.leftMargin: 8
            anchors.rightMargin: 8

            // On démarre sur la page de boot (sélection de borne)
            initialItem: bootPage
        }
    }

    // --- DÉFINITION DES PAGES (Appel des fichiers externes) ---
    // Qt va chercher automatiquement BootPage.qml, StatusPage.qml, etc.

    Component { id: bootPage; BootPage {} }
    Component { id: dashboardPage; DashboardPage {} }
    Component { id: statusPage; StatusPage {} }
    Component { id: vehiculesPage; VehiculesPage {} }
    Component { id: mileagePage; KmPage {} }
    Component { id: settingsPage; SettingsPage {} }
    Component { id: sessionsPage; SessionsPage {} }

    // --- CONNEXIONS C++ (Réception binaire sans JSON) ---
    Connections {
        target: commTemp

        // Réception d'un véhicule (un par un)
        function onVehicleReceived(name, km) {
            vehiclesModel.append({
                "name": name,
                "km": km
            });
        }

        // Réception d'une borne (une par une)
        function onStationReceived(name, kwh, status) {
            stationsModelSource.append({
                "name": name,
                "kwh": kwh,
                "status": status
            });
        }

        // Réception d'une session de charge (une par une)
        function onSessionReceived(borne, vehicule, jours, start, end) {
            sessionsModel.append({
                "station": borne,
                "vehicle": vehicule,
                "days": jours,
                "start": start,
                "end": end
            });

            // Optionnel : Mettre à jour le statut de la borne
            for (var i = 0; i < stationsModelSource.count; i++) {
                var item = stationsModelSource.get(i);
                if (item.name === borne && item.status === "Disponible") {
                    stationsModelSource.setProperty(i, "status", "Programmé");
                }
            }
        }
    }

    // --- INITIALISATION ---
    Component.onCompleted: {
        console.log("Application initialisée et prête.");
    }
}
