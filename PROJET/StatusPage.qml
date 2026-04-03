import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: statusRoot

    property string currentStatus: "Inconnu"

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: statusRoot.currentStatus = window.getSelectedStationStatus()
    }

    function formatTime(totalSeconds) {
        var m = Math.floor(totalSeconds / 60);
        var s = totalSeconds % 60;
        return (m < 10 ? "0" + m : m) + ":" + (s < 10 ? "0" + s : s);
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 16

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                width: 36; height: 36; radius: 18
                color: "#F5F5F5"
                Text { anchors.centerIn: parent; text: "←"; font.pixelSize: 16; color: "#546E7A" }
                MouseArea { anchors.fill: parent; onClicked: stackView.pop() }
            }

            Text {
                text: "Suivi en temps réel"
                font.pixelSize: 20; font.bold: true; color: "#263238"
            }
        }

        Rectangle {
            id: bannerRect
            Layout.fillWidth: true
            height: 90
            radius: 16

            color: {
                if (currentStatus === "En Charge") { return "#E8F5E9"; }
                if (currentStatus === "Programmé") { return "#E3F2FD"; }
                return "#F5F5F5";
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 4

                Text {
                    text: currentStatus.toUpperCase()
                    font.pixelSize: 20; font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                    color: {
                        if (currentStatus === "En Charge") { return "#2E7D32"; }
                        if (currentStatus === "Programmé") { return "#1565C0"; }
                        return "#78909C";
                    }
                }

                Text {
                    text: window.activeStation
                    font.pixelSize: 13; color: "#78909C"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        AppButton {
            id: toggleBtn
            text: currentStatus === "En Charge" ? "Arrêter la charge" : "Lancer la charge"
            isPrimary: currentStatus !== "En Charge"
            Layout.fillWidth: true

            onClicked: {
                var actionDone = false;
                var nouvelEtat = false;

                for (var i = 0; i < stationsModelSource.count; i++) {
                    var item = stationsModelSource.get(i);
                    if (item.name === window.activeStation) {

                        if (!actionDone) {
                            if (item.status === "Disponible") {
                                stationsModelSource.setProperty(i, "status", "En Charge");
                                nouvelEtat = true;
                                actionDone = true;
                            }
                        }
                        if (!actionDone) {
                            if (item.status === "Programmé") {
                                stationsModelSource.setProperty(i, "status", "En Charge");
                                nouvelEtat = true;
                                actionDone = true;
                            }
                        }
                        if (!actionDone) {
                            if (item.status === "En Charge") {
                                var hasSessions = false;
                                for (var k = 0; k < sessionsModel.count; k++) {
                                    var s = sessionsModel.get(k);
                                    if (s.station === window.activeStation) {
                                        hasSessions = true;
                                    }
                                }
                                if (hasSessions) {
                                    stationsModelSource.setProperty(i, "status", "Programmé");
                                }
                                if (!hasSessions) {
                                    stationsModelSource.setProperty(i, "status", "Disponible");
                                    window.sessionDurationSeconds = 0;
                                }
                                nouvelEtat = false;
                                actionDone = true;
                            }
                        }
                    }
                }

                commEsp.marcheForcee(nouvelEtat);
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 16
            color: "white"
            border.color: "#EEEEEE"
            border.width: 1
            visible: currentStatus === "En Charge"

            Rectangle {
                anchors.fill: parent; anchors.topMargin: 3
                radius: parent.radius; color: "#000000"; opacity: 0.04; z: -1
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 16

                Text {
                    text: "Durée écoulée"
                    Layout.alignment: Qt.AlignHCenter
                    color: "#90A4AE"; font.pixelSize: 13
                }

                Text {
                    text: formatTime(window.sessionDurationSeconds)
                    font.pixelSize: 52; font.bold: true; color: "#263238"
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.fillWidth: true; height: 1; color: "#EEEEEE"
                    Layout.topMargin: 8
                }

                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 8

                    ColumnLayout {
                        spacing: 4
                        Text { text: "Énergie"; color: "#90A4AE"; font.pixelSize: 12 }
                        Text {
                            text: window.sessionEnergyKwh.toFixed(2) + " kWh"
                            font.bold: true; font.pixelSize: 18; color: "#263238"
                        }
                    }

                    Item { Layout.fillWidth: true }

                    ColumnLayout {
                        spacing: 4; Layout.alignment: Qt.AlignRight
                        Text { text: "Coût estimé"; color: "#90A4AE"; font.pixelSize: 12 }
                        Text {
                            text: window.sessionCostEuro.toFixed(2) + " €"
                            font.bold: true; font.pixelSize: 18; color: "#1E88E5"
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        Item {
            Layout.fillHeight: true
            visible: currentStatus !== "En Charge"
        }
    }
}
