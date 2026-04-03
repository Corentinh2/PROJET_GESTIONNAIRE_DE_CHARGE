import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: settingsRoot

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

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
                text: "Paramètres"
                font.pixelSize: 22
                font.bold: true
                color: "#263238"
            }
        }

        // Carte paramètres
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 240
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
                spacing: 16

                // Paramètre 1
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Prix de l'électricité"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#263238"
                    }

                    Text {
                        text: "Tarif en €/kWh"
                        font.pixelSize: 11
                        color: "#90A4AE"
                    }

                    AppTextField {
                        id: costInput
                        text: window.userCostPerKwh.toFixed(2)
                        placeholderText: "0.16"
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                        Layout.fillWidth: true
                    }
                }

                // Séparateur
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#EEEEEE"
                }

                // Paramètre 2
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 6

                    Text {
                        text: "Puissance souscrite"
                        font.bold: true
                        font.pixelSize: 14
                        color: "#263238"
                    }

                    Text {
                        text: "En kilowatts (kW)"
                        font.pixelSize: 11
                        color: "#90A4AE"
                    }

                    AppTextField {
                        id: powerInput
                        text: window.userMaxPower.toString()
                        placeholderText: "Ex: 12"
                        inputMethodHints: Qt.ImhDigitsOnly
                        Layout.fillWidth: true
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }

        // Boutons
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            AppButton {
                text: "Annuler"
                isPrimary: false
                Layout.fillWidth: true
                onClicked: stackView.pop()
            }

            AppButton {
                text: "Sauvegarder"
                Layout.fillWidth: true
                onClicked: {
                    var newCost = parseFloat(costInput.text.replace(",", "."));
                    var newPower = parseInt(powerInput.text);
                    if (!isNaN(newCost) && newCost > 0) {
                        window.userCostPerKwh = newCost;
                    }
                    if (!isNaN(newPower) && newPower > 0) {
                        window.userMaxPower = newPower;
                    }
                    stackView.pop();
                }
            }
        }
    }
}
