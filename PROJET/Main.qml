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
    property bool attenteNouvelleListeVehicules: false
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

    // --- STATUTS DE CONNEXION ---
    property string espStatus: "Déconnecté"
    property string raspiStatus: "Déconnecté"

    // --- COMPTEUR SESSIONS ACTIVES ---
    property int activeSessionCount: {
        var count = 0;
        for (var i = 0; i < sessionsModel.count; i++) {
            if (sessionsModel.get(i).station === window.activeStation) { count++; }
        }
        return count;
    }

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
            anchors.bottomMargin: 36
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            initialItem: "BootPage.qml"
        }

        // --- BARRE DE STATUT EN BAS ---
        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 32
            color: "#ECEFF1"
            radius: 0

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 16

                RowLayout {
                    spacing: 5
                    Rectangle {
                        width: 8; height: 8; radius: 4
                        color: window.espStatus === "Connecté" ? "#43A047" : "#E53935"
                    }
                    Text {
                        text: "ESP32 • " + window.espStatus
                        font.pixelSize: 11
                        color: "#546E7A"
                    }
                }

                Rectangle { width: 1; height: 16; color: "#B0BEC5" }

                RowLayout {
                    spacing: 5
                    Rectangle {
                        width: 8; height: 8; radius: 4
                        color: window.raspiStatus === "Connecté" ? "#43A047" : "#E53935"
                    }
                    Text {
                        text: "Raspi • " + window.raspiStatus
                        font.pixelSize: 11
                        color: "#546E7A"
                    }
                }

                Item { Layout.fillWidth: true }
            }
        }

        // --- POPUP ALERTE ---
        Connections {
            target: commEsp
            function onAlerteRecue(type, message) {
                alertPopup.visible = true;
            }
        }

        Rectangle {
            id: alertPopup
            visible: false
            anchors.fill: parent
            color: "#AA000000"
            z: 100

            Rectangle {
                anchors.centerIn: parent
                width: parent.width - 40
                radius: 20
                color: "white"
                height: alertCol.implicitHeight + 40

                ColumnLayout {
                    id: alertCol
                    anchors.centerIn: parent
                    width: parent.width - 40
                    spacing: 16

                    Rectangle {
                        width: 64; height: 64; radius: 32
                        color: "#FFEBEE"
                        Layout.alignment: Qt.AlignHCenter
                        Text { anchors.centerIn: parent; text: "⚠️"; font.pixelSize: 30 }
                    }

                    Text {
                        text: "Alerte détectée"
                        font.bold: true
                        font.pixelSize: 18
                        color: "#C62828"
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: notificationModel.count > 0 ? notificationModel.get(notificationModel.count - 1).message : ""
                        font.pixelSize: 14
                        color: "#546E7A"
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    AppButton {
                        text: "J'ai compris"
                        Layout.fillWidth: true
                        onClicked: alertPopup.visible = false
                    }
                }
            }
        }

    } // fin appFrame

    // ============================================================
    // CONNEXIONS RASPBERRY PI (Stations + Véhicules)
    // ============================================================
    Connections {
        target: commRaspi

        function onConnectionStatusChanged(status) {
            window.raspiStatus = status;
            if (status === "Connecté") {
                sessionsModel.clear();   // ← AJOUTER cette ligne
            }
        }

        function onStationRecue(id, name, kwh, status, ip) {
            stationsModelSource.append({
                                           "stationId": id,
                                           "name": name,
                                           "kwh": kwh,
                                           "status": status,
                                           "ip": ip
                                       });
        }

        function onVehiculeRecu(id, name, km) {
            if (window.attenteNouvelleListeVehicules) {
                vehiclesModel.clear();
                window.attenteNouvelleListeVehicules = false;
            }
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

        function onConnectionStatusChanged(status) {
            window.espStatus = status;
            if (status === "Connecté") {
                stationsModelSource.clear();
            }
        }

        function onClearCalendriers() {
            sessionsModel.clear();
            console.log("[MAIN] Liste des calendriers vidée (nouvelle liste en cours)");
        }

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
            var msg = "";
            if (type === "surchauffe") {
                msg = "La charge s'est arrêtée pour la raison suivante : Température trop élevée ! ";
            } else {
                msg = "La charge s'est arrêtée pour la raison suivante : Tension trop élevée !";
            }
            notificationModel.append({
                                         "type": type,
                                         "message": msg
                                     });
            console.log("[ALERTE ESP32] " + type + " : " + msg);
        }
    }

}
