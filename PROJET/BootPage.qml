import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique

Item {
    id: bootRoot

    ColumnLayout {
        anchors.fill: parent // <--- LA CORRECTION EST ICI
        anchors.margins: 20
        spacing: 30

        Item { Layout.fillHeight: true } // Espaceur

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 10
            Text {
                text: "Gestionnaire de charge"
                font.pixelSize: 24; font.bold: true; color: "#2979FF"
                Layout.alignment: Qt.AlignHCenter
            }
            Text {
                text: "Choisissez une borne pour commencer"
                font.pixelSize: 14; color: "#666"
                Layout.alignment: Qt.AlignHCenter
            }
        }

        // Liste des bornes reçues du serveur
        ListView {
            id: stationList
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            clip: true
            spacing: 12
            model: stationsModelSource // Défini dans Main.qml

            delegate: Rectangle {
                width: stationList.width
                height: 70
                radius: 12
                color: "white"
                border.color: "#EEE"
                border.width: 1

                // Effet d'ombre/survol simple
                Rectangle {
                    anchors.fill: parent; radius: 12; color: "#000"; opacity: ma.pressed ? 0.05 : 0
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15
                    Text { text: "🔌"; font.pixelSize: 22 }
                    ColumnLayout {
                        Layout.fillWidth: true
                        Text { text: name; font.bold: true; font.pixelSize: 16 }
                        Text { text: kwh; font.pixelSize: 12; color: "#888" }
                    }
                    Text { text: ">"; color: "#2979FF"; font.bold: true }
                }

                MouseArea {
                    id: ma
                    anchors.fill: parent
                    onClicked: Logique.selectActiveStation(name)
                }
            }
        }

        Item { Layout.fillHeight: true }
    }
}
