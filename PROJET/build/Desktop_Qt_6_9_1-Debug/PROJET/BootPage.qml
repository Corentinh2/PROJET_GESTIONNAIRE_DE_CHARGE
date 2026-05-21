import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "js/BootPage.js" as BootJS

Item {
    id: bootRoot

    // --- BANNIÈRE D'ALERTE ---
    Rectangle {
        id: alertBanner
        width: parent.width
        height: 56
        z: 10
        visible: false
        color: BootJS.alertTypeColor(alertTypeText.text)
        radius: 12

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 10

            Text { text: "⚠️"; font.pixelSize: 20 }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 1

                Text {
                    id: alertTypeText
                    font.pixelSize: 11
                    font.bold: true
                    color: "#37474F"
                    text: ""
                }
                Text {
                    id: alertMessageText
                    font.pixelSize: 12
                    color: "#263238"
                    text: ""
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { alertBanner.visible = false; }
        }
    }

    Connections {
        target: commEsp
        function onAlerteRecue(type, message) {
            alertTypeText.text = type;
            alertMessageText.text = message;
            alertBanner.visible = true;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: {
            if (alertBanner.visible) { return 60; }
            return 0;
        }
        anchors.margins: 20
        spacing: 0

        Item { Layout.preferredHeight: 24 }

        // --- EN-TÊTE ---
        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8

            Rectangle {
                width: 72
                height: 72
                radius: 36
                Layout.alignment: Qt.AlignHCenter
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "#1565C0" }
                    GradientStop { position: 1.0; color: "#42A5F5" }
                }

                Text {
                    anchors.centerIn: parent
                    text: "⚡"
                    font.pixelSize: 30
                }
            }

            Item { height: 6 }

            Text {
                text: "Gestionnaire de charge"
                font.pixelSize: 22
                font.bold: true
                color: "#1A237E"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: "Sélectionnez une borne pour commencer"
                font.pixelSize: 13
                color: "#78909C"
                Layout.alignment: Qt.AlignHCenter
            }
        }

        Item { Layout.preferredHeight: 28 }

        // --- LABEL SECTION ---
        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 2

            Text {
                text: "BORNES DISPONIBLES"
                font.pixelSize: 11
                font.bold: true
                font.letterSpacing: 1.5
                color: "#90A4AE"
            }

            Item { Layout.fillWidth: true }

            Text {
                text: stationsModelSource.count + " borne(s)"
                font.pixelSize: 11
                color: "#B0BEC5"
            }
        }

        Item { Layout.preferredHeight: 10 }

        // --- LISTE DES BORNES ---
        ListView {
            id: stationList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10
            model: stationsModelSource

            delegate: Rectangle {
                width: stationList.width
                height: 76
                radius: 16
                color: "white"
                border.color: BootJS.statusBorderColor(status)
                border.width: 2

                Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 3
                    radius: parent.radius
                    color: "#000000"
                    opacity: 0.03
                    z: -1
                }

                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    color: "#000000"
                    opacity: {
                        if (ma.pressed) { return 0.04; }
                        return 0;
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 14
                    spacing: 12

                    Rectangle {
                        width: 46
                        height: 46
                        radius: 23
                        color: BootJS.iconBgColor(status)
                        Layout.alignment: Qt.AlignVCenter

                        Text {
                            anchors.centerIn: parent
                            text: "🔌"
                            font.pixelSize: 22
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                        spacing: 4

                        Text {
                            text: name
                            font.bold: true
                            font.pixelSize: 15
                            color: "#263238"
                        }

                        RowLayout {
                            spacing: 6

                            Text {
                                text: kwh + " W"
                                font.pixelSize: 11
                                color: "#90A4AE"
                            }

                            Text { text: "·"; color: "#B0BEC5"; font.pixelSize: 11 }

                            Rectangle {
                                height: 18
                                width: statusTxt.implicitWidth + 14
                                radius: 9
                                color: BootJS.statusBadgeColor(status)

                                RowLayout {
                                    anchors.centerIn: parent
                                    spacing: 4

                                    Rectangle {
                                        width: 6; height: 6; radius: 3
                                        color: BootJS.statusDotColor(status)
                                    }

                                    Text {
                                        id: statusTxt
                                        text: BootJS.statusLabel(status)
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: BootJS.statusDotColor(status)
                                    }
                                }
                            }
                        }
                    }

                    Text {
                        text: "›"
                        color: "#1E88E5"
                        font.pixelSize: 26
                        Layout.alignment: Qt.AlignVCenter
                    }
                }

                MouseArea {
                    id: ma
                    anchors.fill: parent
                    onClicked: {
                        window.activeStation = name;
                        commEsp.connexion(ip);
                        stackView.push("VehiculesPage.qml");
                    }
                }
            }
        }

        Item { Layout.preferredHeight: 10 }
    }
}
