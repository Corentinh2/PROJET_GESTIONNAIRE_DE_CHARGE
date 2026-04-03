import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: bootRoot

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 0

        Item { Layout.preferredHeight: 40 }

        // Logo / En-tête
        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 6

            // Icône décorative
            Rectangle {
                width: 64
                height: 64
                radius: 32
                color: "#E3F2FD"
                Layout.alignment: Qt.AlignHCenter

                Text {
                    anchors.centerIn: parent
                    text: "⚡"
                    font.pixelSize: 28
                }
            }

            Item { height: 8 }

            Text {
                text: "Gestionnaire de charge"
                font.pixelSize: 22
                font.bold: true
                color: "#263238"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Sélectionnez une borne pour commencer"
                font.pixelSize: 13
                color: "#78909C"
                Layout.alignment: Qt.AlignHCenter
            }
        }

        Item { Layout.preferredHeight: 30 }

        // Label section
        Text {
            text: "Bornes disponibles"
            font.pixelSize: 12
            font.bold: true
            color: "#90A4AE"
            font.letterSpacing: 1
            Layout.leftMargin: 4
        }

        Item { Layout.preferredHeight: 10 }

        // Liste des bornes
        ListView {
            id: stationList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10
            model: stationsModelSource

            delegate: Rectangle {
                width: stationList.width
                height: 72
                radius: 14
                color: "white"
                border.color: "#EEEEEE"
                border.width: 1

                // Ombre douce
                Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 3
                    radius: parent.radius
                    color: "#000000"
                    opacity: 0.04
                    z: -1
                }

                // Effet au clic
                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    color: "#000000"
                    opacity: ma.pressed ? 0.04 : 0
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    spacing: 12

                    // Icône borne
                    Rectangle {
                        width: 44
                        height: 44
                        radius: 22
                        color: "#E8F5E9"
                        Layout.alignment: Qt.AlignVCenter

                        Text {
                            anchors.centerIn: parent
                            text: "🔌"
                            font.pixelSize: 20
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        spacing: 2

                        Text {
                            text: name
                            font.bold: true
                            font.pixelSize: 15
                            color: "#263238"
                        }

                        Text {
                            text: kwh
                            font.pixelSize: 12
                            color: "#90A4AE"
                        }
                    }

                    Text {
                        text: "›"
                        color: "#1E88E5"
                        font.pixelSize: 24
                        Layout.alignment: Qt.AlignVCenter
                    }
                }

                MouseArea {
                    id: ma
                    anchors.fill: parent
                    onClicked: {
                        window.activeStation = name;
                        stackView.push("VehiculesPage.qml");
                    }
                }
            }
        }

        Item { Layout.preferredHeight: 10 }
    }
}
