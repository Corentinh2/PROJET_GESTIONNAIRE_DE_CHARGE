import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique

Item {
    id: vehiclesRoot

    // --- VUE PRINCIPALE (LA LISTE) ---
    ColumnLayout {
        anchors.fill: parent // <--- CORRECTION 1 : On remplit l'Item racine
        anchors.margins: 15  // <--- CORRECTION 2 : On décolle des bords
        spacing: 15

        RowLayout {
            Layout.fillWidth: true
            RoundButton { text: "←"; onClicked: stackView.pop() }
            Text { text: "Véhicules"; font.pixelSize: 20; font.bold: true }
        }

        AppButton {
            text: "+ Ajouter Véhicule"
            Layout.fillWidth: true // Pour que le bouton prenne bien toute la largeur
            onClicked: stackView.push(addVehicleForm) // On appelle le composant interne
        }

        ListView {
            Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10
            model: vehiclesModel
            clip: true

            delegate: Rectangle {
                width: parent.width; height: 70; radius: 8; color: "white"; border.color: "#DDD"; border.width: 1

                MouseArea {
                    anchors.fill: parent;
                    onClicked: Logique.selectVehicle(name)
                }

                RowLayout {
                    anchors.fill: parent; anchors.margins: 10

                    ColumnLayout {
                        Layout.fillWidth: true
                        Text { text: "🚗 " + name; font.bold: true; font.pixelSize: 16 }
                        Text { text: km + " km"; color: "#666"; font.pixelSize: 12 }
                    }

                    AppButton {
                        text: "X"; isPrimary: false; textColor: "red";
                        Layout.preferredWidth: 40;
                        onClicked: Logique.removeVehicle(index)
                    }
                }
            }
        }
    }

    // --- SOUS-PAGE : LE FORMULAIRE (DANS LE MÊME FICHIER) ---
    Component {
        id: addVehicleForm
        Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15 // <--- CORRECTION 3 : Marges pour le formulaire aussi
                spacing: 20

                Text { text: "Nouveau Véhicule"; font.bold: true; font.pixelSize: 20 }

                AppTextField {
                    id: vName;
                    placeholderText: "Modèle"
                    Layout.fillWidth: true
                }

                AppTextField {
                    id: vKm;
                    placeholderText: "KM";
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Enregistrer"
                        Layout.fillWidth: true
                        onClicked: {
                            commTemp.addVehicleToDb(vName.text, vKm.text);
                            stackView.pop();
                        }
                    }
                }
                Item { Layout.fillHeight: true }
            }
        }
    }
}
