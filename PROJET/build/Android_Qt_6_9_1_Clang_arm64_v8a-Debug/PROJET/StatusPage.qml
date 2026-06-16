import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "js/StatusPage.js" as StatusJS

Item {
    id: statusRoot

    property string currentStatus: "Inconnu"

    Timer {
        interval: 500
        running: true
        repeat: true
        onTriggered: statusRoot.currentStatus = window.getSelectedStationStatus()
    }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 15; spacing: 16

        // En-tête
        RowLayout {
            Layout.fillWidth: true; spacing: 10

            Rectangle {
                width: 36; height: 36; radius: 18; color: "#F5F5F5"
                Text { anchors.centerIn: parent; text: "←"; font.pixelSize: 16; color: "#546E7A" }
                MouseArea { anchors.fill: parent; onClicked: stackView.pop() }
            }

            Text { text: "Suivi en temps réel"; font.pixelSize: 20; font.bold: true; color: "#263238" }
        }

        // Bannière statut
        Rectangle {
            Layout.fillWidth: true
            height: 100; radius: 20
            color: StatusJS.statusBgColor(currentStatus)
            border.color: StatusJS.statusBorderColor(currentStatus)
            border.width: 1

            RowLayout {
                anchors.centerIn: parent; spacing: 16

                Rectangle {
                    width: 56; height: 56; radius: 28
                    color: Qt.darker(StatusJS.statusBgColor(currentStatus), 1.08)
                    Text { anchors.centerIn: parent; text: StatusJS.statusIcon(currentStatus); font.pixelSize: 26 }
                }

                ColumnLayout {
                    spacing: 4
                    Text {
                        text: currentStatus.toUpperCase()
                        font.pixelSize: 20; font.bold: true
                        color: StatusJS.statusColor(currentStatus)
                        font.letterSpacing: 1
                    }
                    Text { text: window.activeStation; font.pixelSize: 13; color: "#78909C" }
                }
            }
        }

        // Bouton lancer/arrêter
        AppButton {
            id: toggleBtn
            text: StatusJS.buttonText(currentStatus)
            isPrimary: StatusJS.buttonPrimary(currentStatus)
            Layout.fillWidth: true
            onClicked: {
                var nouvelEtat = false;
                for (var i = 0; i < stationsModelSource.count; i++) {
                    var item = stationsModelSource.get(i);
                    if (item.name === window.activeStation) {
                        if (item.status === "En Charge") {
                            if (StatusJS.verifierSessions()) {
                                stationsModelSource.setProperty(i, "status", "Programmé");
                            } else {
                                stationsModelSource.setProperty(i, "status", "Disponible");
                                window.sessionDurationSeconds = 0;
                            }
                            nouvelEtat = false;
                        } else {
                            stationsModelSource.setProperty(i, "status", "En Charge");
                            nouvelEtat = true;
                        }
                    }
                }
                commEsp.marcheForcee(nouvelEtat);
            }
        }

        Item { Layout.fillHeight: true }
    }
}
