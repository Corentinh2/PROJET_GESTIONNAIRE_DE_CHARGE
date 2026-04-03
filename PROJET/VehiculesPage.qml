import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: vehiclesRoot

    // Component.onCompleted: {
    //     vehiclesModel.clear();
    //     commRaspi.obtenirVehicule();
    // }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 14

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
                text: "Véhicules"
                font.pixelSize: 20
                font.bold: true
                color: "#263238"
            }
        }

        AppButton {
            text: "+ Ajouter un véhicule"
            Layout.fillWidth: true
            onClicked: stackView.push(addVehicleForm)
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            model: vehiclesModel
            clip: true

            delegate: Rectangle {
                width: parent.width
                height: 72
                radius: 14
                color: "white"
                border.color: "#EEEEEE"
                border.width: 1

                Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 3
                    radius: parent.radius
                    color: "#000000"
                    opacity: 0.04
                    z: -1
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        window.selectedVehicle = name;
                        stackView.push("KmPage.qml");
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 14
                    anchors.rightMargin: 10
                    spacing: 12

                    Rectangle {
                        width: 44; height: 44; radius: 22
                        color: "#E3F2FD"
                        Layout.alignment: Qt.AlignVCenter
                        Text { anchors.centerIn: parent; text: "🚗"; font.pixelSize: 20 }
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
                            text: km + " km"
                            color: "#90A4AE"
                            font.pixelSize: 12
                        }
                    }

                    // Suppression par ID
                    Rectangle {
                        width: 34; height: 34; radius: 17
                        color: "#FFEBEE"
                        Layout.alignment: Qt.AlignVCenter

                        Text {
                            anchors.centerIn: parent
                            text: "✕"
                            color: "#E53935"
                            font.pixelSize: 14
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (name === window.selectedVehicle) {
                                    window.selectedVehicle = "";
                                }
                                commRaspi.supprimerVehicule(vehicleId);
                                vehiclesModel.remove(index);
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: addVehicleForm
        Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Text {
                    text: "Nouveau véhicule"
                    font.bold: true
                    font.pixelSize: 20
                    color: "#263238"
                }

                Rectangle {
                    Layout.fillWidth: true; height: 160; radius: 16
                    color: "white"; border.color: "#EEEEEE"; border.width: 1

                    Rectangle {
                        anchors.fill: parent; anchors.topMargin: 3
                        radius: parent.radius; color: "#000000"; opacity: 0.04; z: -1
                    }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 16; spacing: 12

                        AppTextField {
                            id: vName
                            placeholderText: "Modèle du véhicule"
                            Layout.fillWidth: true
                        }

                        AppTextField {
                            id: vKm
                            placeholderText: "Kilométrage actuel"
                            inputMethodHints: Qt.ImhDigitsOnly
                            Layout.fillWidth: true
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true; spacing: 12

                    AppButton {
                        text: "Annuler"; isPrimary: false; Layout.fillWidth: true
                        onClicked: stackView.pop()
                    }

                    AppButton {
                        text: "Enregistrer"; Layout.fillWidth: true
                        onClicked: {
                            commRaspi.ajouterVehicule(vName.text, vKm.text);
                            stackView.pop();
                        }
                    }
                }
            }
        }
    }
}
