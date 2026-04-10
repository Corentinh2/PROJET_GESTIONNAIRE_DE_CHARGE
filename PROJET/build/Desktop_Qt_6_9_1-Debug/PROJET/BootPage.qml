import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: bootRoot

    // --- BANNIÈRE D'ALERTE ---
    Rectangle {
        id: alertBanner
        width: parent.width
        height: 56
        z: 10
        visible: false
        color: alertBanner.alertTypeColor(alertTypeText.text)

        function alertTypeColor(type) {
            if (type === "surcharge")   { return "#FFCDD2"; }
            if (type === "temperature") { return "#FFE0B2"; }
            return "#E3F2FD";
        }

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
        anchors.topMargin: alertBanner.visible ? 60 : 0
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
                border.color: statusBorderColor(status)
                border.width: 2

                function statusBorderColor(s) {
                    if (s === "En Charge") { return "#EF9A9A"; }
                    if (s === "Programmé") { return "#90CAF9"; }
                    return "#C8E6C9";
                }

                function statusDotColor(s) {
                    if (s === "En Charge") { return "#E53935"; }
                    if (s === "Programmé") { return "#1E88E5"; }
                    return "#43A047";
                }

                function statusLabel(s) {
                    if (s === "En Charge") { return "En charge"; }
                    if (s === "Programmé") { return "Programmé"; }
                    return "Disponible";
                }

                function iconBgColor(s) {
                    if (s === "En Charge") { return "#FFEBEE"; }
                    if (s === "Programmé") { return "#E3F2FD"; }
                    return "#E8F5E9";
                }

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
                    opacity: ma.pressed ? 0.04 : 0
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
                        color: iconBgColor(status)
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
                                text: kwh
                                font.pixelSize: 11
                                color: "#90A4AE"
                            }

                            Text { text: "·"; color: "#B0BEC5"; font.pixelSize: 11 }

                            Rectangle {
                                height: 18
                                width: statusTxt.implicitWidth + 14
                                radius: 9
                                color: {
                                    if (status === "En Charge") { return "#FFEBEE"; }
                                    if (status === "Programmé") { return "#E3F2FD"; }
                                    return "#E8F5E9";
                                }

                                RowLayout {
                                    anchors.centerIn: parent
                                    spacing: 4

                                    Rectangle {
                                        width: 6; height: 6; radius: 3
                                        color: statusDotColor(status)
                                    }

                                    Text {
                                        id: statusTxt
                                        text: statusLabel(status)
                                        font.pixelSize: 10
                                        font.bold: true
                                        color: statusDotColor(status)
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
                        commEsp.setIp(ip);
                        stackView.push("VehiculesPage.qml");
                    }
                }
            }
        }

        Item { Layout.preferredHeight: 10 }
    }
}
