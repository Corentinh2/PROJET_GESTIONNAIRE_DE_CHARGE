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

    function statusColor() {
        if (currentStatus === "En Charge") { return "#2E7D32"; }
        if (currentStatus === "Programmé") { return "#1565C0"; }
        return "#78909C";
    }

    function statusBgColor() {
        if (currentStatus === "En Charge") { return "#E8F5E9"; }
        if (currentStatus === "Programmé") { return "#E3F2FD"; }
        return "#F5F5F5";
    }

    function statusIcon() {
        if (currentStatus === "En Charge") { return "⚡"; }
        if (currentStatus === "Programmé") { return "📅"; }
        return "🔌";
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 16

        // En-tête
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

        // Bannière statut
        Rectangle {
            Layout.fillWidth: true
            height: 100
            radius: 20
            color: statusBgColor()
            border.color: Qt.darker(statusBgColor(), 1.05)
            border.width: 1

            RowLayout {
                anchors.centerIn: parent
                spacing: 16

                Rectangle {
                    width: 56; height: 56; radius: 28
                    color: Qt.darker(statusBgColor(), 1.08)

                    Text {
                        anchors.centerIn: parent
                        text: statusIcon()
                        font.pixelSize: 26
                    }
                }

                ColumnLayout {
                    spacing: 4

                    Text {
                        text: currentStatus.toUpperCase()
                        font.pixelSize: 20; font.bold: true
                        color: statusColor()
                        font.letterSpacing: 1
                    }

                    Text {
                        text: window.activeStation
                        font.pixelSize: 13; color: "#78909C"
                    }
                }
            }
        }

        // Bouton lancer/arrêter
        AppButton {
            id: toggleBtn
            text: currentStatus === "En Charge" ? "⏹  Arrêter la charge" : "▶  Lancer la charge"
            isPrimary: currentStatus !== "En Charge"
            Layout.fillWidth: true

            onClicked: {
                var nouvelEtat = false;

                for (var i = 0; i < stationsModelSource.count; i++) {
                    var item = stationsModelSource.get(i);
                    if (item.name === window.activeStation) {
                        if (item.status === "En Charge") {
                            var hasSessions = false;
                            for (var k = 0; k < sessionsModel.count; k++) {
                                if (sessionsModel.get(k).station === window.activeStation) {
                                    hasSessions = true;
                                }
                            }
                            stationsModelSource.setProperty(i, "status", hasSessions ? "Programmé" : "Disponible");
                            if (!hasSessions) { window.sessionDurationSeconds = 0; }
                            nouvelEtat = false;
                        } else {
                            // Disponible, Programmé ou Inconnu → on lance
                            stationsModelSource.setProperty(i, "status", "En Charge");
                            nouvelEtat = true;
                        }
                    }
                }

                commEsp.marcheForcee(nouvelEtat);
            }
        }

        // Carte données en charge
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: visible
            implicitHeight: visible ? 200 : 0
            radius: 20
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

                // Barre de progression
                Rectangle {
                    Layout.fillWidth: true
                    height: 6; radius: 3
                    color: "#F5F5F5"

                    Rectangle {
                        width: Math.min(parent.width, (window.sessionEnergyKwh / 60.0) * parent.width)
                        height: parent.height
                        radius: parent.radius
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: "#42A5F5" }
                            GradientStop { position: 1.0; color: "#1E88E5" }
                        }
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#EEEEEE" }

                RowLayout {
                    Layout.fillWidth: true

                    Rectangle {
                        Layout.fillWidth: true
                        height: 70; radius: 14
                        color: "#E8F5E9"

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                text: "Énergie"
                                color: "#388E3C"; font.pixelSize: 11; font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: window.sessionEnergyKwh.toFixed(2) + " kWh"
                                font.bold: true; font.pixelSize: 17; color: "#2E7D32"
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    Item { width: 10 }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 70; radius: 14
                        color: "#E3F2FD"

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 4

                            Text {
                                text: "Coût estimé"
                                color: "#1565C0"; font.pixelSize: 11; font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: window.sessionCostEuro.toFixed(2) + " €"
                                font.bold: true; font.pixelSize: 17; color: "#1E88E5"
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // Message quand pas en charge
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: visible
            implicitHeight: visible ? 200 : 0
            visible: currentStatus !== "En Charge"
            spacing: 10

            Item { Layout.fillHeight: true }

            Text {
                text: currentStatus === "Programmé" ? "Charge programmée" : "Borne disponible"
                font.pixelSize: 15; font.bold: true
                color: "#90A4AE"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Appuyez sur \"Lancer la charge\"\npour démarrer"
                font.pixelSize: 12; color: "#B0BEC5"
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            Item { Layout.fillHeight: true }
        }
    }
}
