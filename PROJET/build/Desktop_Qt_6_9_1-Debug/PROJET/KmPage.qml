import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: mileageRootStep

    function getMileage(vName) {
        var kmVal = 0;
        for (var i = 0; i < vehiclesModel.count; i++) {
            var item = vehiclesModel.get(i);
            if (item.name === vName) {
                kmVal = parseInt(item.km);
            }
        }
        return kmVal;
    }

    property int currentVehicleKm: getMileage(window.selectedVehicle)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // En-tête
        Text {
            text: "Validation kilométrage"
            font.bold: true
            font.pixelSize: 20
            color: "#263238"
        }

        // Carte véhicule
        Rectangle {
            Layout.fillWidth: true
            height: 210
            radius: 16
            color: "white"
            border.color: "#EEEEEE"
            border.width: 1

            // Ombre
            Rectangle {
                anchors.fill: parent
                anchors.topMargin: 3
                radius: parent.radius
                color: "#000000"
                opacity: 0.04
                z: -1
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 12

                // Icône véhicule
                Rectangle {
                    width: 48
                    height: 48
                    radius: 24
                    color: "#E3F2FD"
                    Layout.alignment: Qt.AlignHCenter

                    Text {
                        anchors.centerIn: parent
                        text: "🚗"
                        font.pixelSize: 22
                    }
                }

                Text {
                    text: window.selectedVehicle
                    font.bold: true
                    font.pixelSize: 18
                    color: "#1E88E5"
                    Layout.alignment: Qt.AlignHCenter
                }

                // Séparateur
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#EEEEEE"
                }

                Text {
                    text: "Dernier relevé : " + mileageRootStep.currentVehicleKm + " km"
                    font.pixelSize: 13
                    font.bold: true
                    color: "#546E7A"
                    Layout.alignment: Qt.AlignHCenter
                }

                AppTextField {
                    id: kmInput
                    placeholderText: "Nouveau kilométrage"
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.fillWidth: true
                }

                Text {
                    visible: kmInput.text !== "" && parseInt(kmInput.text) < mileageRootStep.currentVehicleKm
                    text: "⚠ Le kilométrage ne peut pas être inférieur à l'ancien"
                    color: "#E53935"
                    font.pixelSize: 11
                    font.bold: true
                }
            }
        }

        Item { Layout.fillHeight: true }

        // Boutons
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            AppButton {
                text: "Retour"
                isPrimary: false
                Layout.fillWidth: true
                onClicked: stackView.pop()
            }

            AppButton {
                text: "Valider ✓"
                Layout.fillWidth: true
                enabled: kmInput.text !== "" && parseInt(kmInput.text) >= mileageRootStep.currentVehicleKm
                onClicked: {
                    var kmSaisi = parseInt(kmInput.text);
                    window.selectedMileage = kmSaisi;
                    for (var v = 0; v < vehiclesModel.count; v++) {
                        var vItem = vehiclesModel.get(v);
                        if (vItem.name === window.selectedVehicle) {
                            vehiclesModel.setProperty(v, "km", kmSaisi.toString());
                        }
                    }
                    stackView.replace(null, "DashboardPage.qml");
                }
            }
        }
    }
}
