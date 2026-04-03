import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window

    width: 360
    height: 720
    visible: true
    title: "Gestionnaire de charge"
    color: "#CFD8DC"
    font.family: "Segoe UI"

    // --- PROPRIÉTÉS GLOBALES ---
    property string activeStation: ""
    property string selectedVehicle: ""
    property bool isBookingFlow: false
    property int selectedMileage: 0

    // Synchronisation borne/véhicule avec commEsp
    onActiveStationChanged: commEsp.activeBorne = activeStation
    onSelectedVehicleChanged: commEsp.activeVehicule = selectedVehicle

    property string bookingDays: ""
    property string bookingStart: ""
    property string bookingEnd: ""

    property double userCostPerKwh: 0.16
    property int userMaxPower: 12

    property int sessionDurationSeconds: 0
    property double sessionEnergyKwh: 0.0
    property double sessionCostEuro: 0.0

    function getSelectedStationStatus() {
        var status = "Inconnu";
        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.activeStation) {
                status = item.status;
            }
        }
        return status;
    }

    // --- MODÈLES DE DONNÉES ---
    ListModel { id: notificationModel }
    ListModel { id: sessionsModel }
    ListModel { id: vehiclesModel }
    ListModel { id: stationsModelSource }

    // --- MOTEUR DE SIMULATION ---
    Timer {
        id: globalChargingTimer
        interval: 1000
        repeat: true
        running: (window.activeStation !== "") && (window.getSelectedStationStatus() === "En Charge")

        onTriggered: {
            window.sessionDurationSeconds += 1;
            var energyAdded = window.userMaxPower / 3600.0;
            window.sessionEnergyKwh += energyAdded;
            window.sessionCostEuro = window.sessionEnergyKwh * window.userCostPerKwh;
        }
    }

    // --- STRUCTURE VISUELLE ---
    Rectangle {
        id: appFrame
        anchors.fill: parent
        anchors.margins: 3
        radius: 28
        color: "#F5F7FA"
        border.color: "#B0BEC5"
        border.width: 3
        clip: true

        StackView {
            id: stackView
            anchors.fill: parent
            anchors.topMargin: 14
            anchors.bottomMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            initialItem: "BootPage.qml"
        }
    }

    // ============================================================
    // CONNEXIONS RASPBERRY PI (Stations + Véhicules)
    // ============================================================
    Connections {
        target: commRaspi

        function onStationRecue(id, name, kwh, status) {
            stationsModelSource.append({
                "stationId": id,
                "name": name,
                "kwh": kwh,
                "status": status
            });
        }

        function onVehiculeRecu(id, name, km) {
            vehiclesModel.append({
                "vehicleId": id,
                "name": name,
                "km": String(km)
            });
        }
    }

    // ============================================================
    // CONNEXIONS ESP32 (Calendriers + Alertes)
    // ============================================================
    Connections {
        target: commEsp

        function onCalendrierRecu(id, borne, vehicule, jours, start, end) {
            console.log("[MAIN] Calendrier ajouté : id=" + id + " / " + borne + " / " + vehicule + " / " + jours);
            sessionsModel.append({
                "sessionId": id,
                "station": borne,
                "vehicle": vehicule,
                "days": jours,
                "start": start,
                "end": end
            });

            for (var i = 0; i < stationsModelSource.count; i++) {
                var item = stationsModelSource.get(i);
                if (item.name === borne && item.status === "Disponible") {
                    stationsModelSource.setProperty(i, "status", "Programmé");
                }
            }
        }

        function onAlerteRecue(type, message) {
            notificationModel.append({
                "type": type,
                "message": message
            });
            console.log("[ALERTE ESP32] " + type + " : " + message);
        }
    }

    Component.onCompleted: {
        console.log("Application initialisée et prête.");
    }
}
