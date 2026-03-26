import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique

Item {
    id: mileageRootStep

    // Récupération sécurisée de l'ancien KM via JS
    property int currentVehicleKm: Logique.getVehicleMileage(window.selectedVehicle)

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 25

        Text { text: "Étape Finale : Validation"; font.bold: true; font.pixelSize: 20; color: "#2979FF" }

        Rectangle {
            Layout.fillWidth: true; height: 200; radius: 12; color: "white"; border.color: "#EEE"; border.width: 1
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 10
                Text { text: "Mise à jour pour :"; color: "#666"; Layout.alignment: Qt.AlignHCenter }
                Text { text: window.selectedVehicle; font.bold: true; font.pixelSize: 20; color: "#2979FF"; Layout.alignment: Qt.AlignHCenter }
                Text { text: "Dernier relevé : " + mileageRootStep.currentVehicleKm + " km"; font.bold: true; Layout.topMargin: 10 }

                AppTextField {
                    id: kmInput
                    placeholderText: "Saisir nouveau kilométrage"
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.fillWidth: true
                }

                Text {
                    visible: kmInput.text !== "" && parseInt(kmInput.text) < mileageRootStep.currentVehicleKm
                    text: "⚠️ Erreur : Ne peut être inférieur à l'ancien."
                    color: "#FF5252"; font.pixelSize: 12; font.bold: true
                }
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.fillWidth: true; spacing: 15
            AppButton {
                text: "Annuler";
                isPrimary: false;
                Layout.fillWidth: true;
                onClicked: {
                    Logique.cancelBooking();
                    stackView.pop(); // Retour aux véhicules
                }
            }
            AppButton {
                text: "Finaliser ✓"
                Layout.fillWidth: true
                enabled: kmInput.text !== "" && parseInt(kmInput.text) >= mileageRootStep.currentVehicleKm
                onClicked: {
                    Logique.finalizeBooking(kmInput.text);
                }
            }
        }
    }
}
