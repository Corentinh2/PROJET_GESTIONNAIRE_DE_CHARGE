import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique

Item {
    id: statusRoot

    // Propriété locale pour rafraîchir l'interface
    property string currentStatus: "Inconnu"

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: statusRoot.currentStatus = Logique.getSelectedStationStatus()
    }

    // Fonction interne pour le formatage du temps
    function formatTime(totalSeconds) {
        var m = Math.floor(totalSeconds / 60);
        var s = totalSeconds % 60;
        return (m < 10 ? "0" + m : m) + ":" + (s < 10 ? "0" + s : s);
    }

    ColumnLayout {
        anchors.fill: parent // <--- CORRECTION 1 : Indispensable pour que ça prenne tout l'écran !
        anchors.margins: 15
        spacing: 20

        // En-tête
        RowLayout {
            Layout.fillWidth: true
            RoundButton {
                text: "←"
                onClicked: stackView.pop()
            }
            Text {
                text: "Suivi en temps réel"
                font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10
            }
        }

        // Bannière d'état dynamique
        Rectangle {
            id: bannerRect // <--- ASTUCE : On donne un id pour cibler facilement ses couleurs
            Layout.fillWidth: true
            height: 100
            radius: 15
            // On change de couleur selon l'état
            color: currentStatus === "En Charge" ? "#E8F5E9" : (currentStatus === "Programmé" ? "#E3F2FD" : "#F5F5F5")
            border.color: currentStatus === "En Charge" ? "#4CAF50" : (currentStatus === "Programmé" ? "#2196F3" : "#CCC")
            border.width: 2

            ColumnLayout {
                anchors.centerIn: parent
                Text {
                    text: currentStatus.toUpperCase()
                    font.pixelSize: 22; font.bold: true
                    color: bannerRect.border.color // <--- CORRECTION 2 : Plus d'erreur "undefined"
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    text: window.activeStation
                    font.pixelSize: 14; color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        // Bouton de commande
        AppButton {
            text: currentStatus === "En Charge" ? "ARRÊTER LA CHARGE" : "LANCER LA CHARGE"
            isPrimary: currentStatus !== "En Charge"
            // On force une couleur rouge si on veut stopper
            background: Rectangle {
                color: statusRoot.currentStatus === "En Charge" ? "#FF5252" : "#2979FF"
                radius: 8
            }
            onClicked: Logique.toggleForcedMode()
        }

        // Statistiques de session (Visible seulement quand ça charge)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 15
            color: "white"
            border.color: "#EEE"
            visible: currentStatus === "En Charge"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 25
                spacing: 20

                Text { text: "Durée écoulée"; Layout.alignment: Qt.AlignHCenter; color: "#888" }

                Text {
                    text: formatTime(window.sessionDurationSeconds)
                    font.pixelSize: 56; font.bold: true; color: "#333"
                    Layout.alignment: Qt.AlignHCenter
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.topMargin: 20

                    ColumnLayout {
                        Text { text: "Énergie"; color: "#888"; font.pixelSize: 12 }
                        Text { text: window.sessionEnergyKwh.toFixed(2) + " kWh"; font.bold: true; font.pixelSize: 18 }
                    }

                    Item { Layout.fillWidth: true }

                    ColumnLayout {
                        Layout.alignment: Qt.AlignRight
                        Text { text: "Coût estimé"; color: "#888"; font.pixelSize: 12 }
                        Text { text: window.sessionCostEuro.toFixed(2) + " €"; font.bold: true; font.pixelSize: 18; color: "#2979FF" }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // Espaceur si non en charge
        Item {
            Layout.fillHeight: true
            visible: currentStatus !== "En Charge"
        }
    }
}
