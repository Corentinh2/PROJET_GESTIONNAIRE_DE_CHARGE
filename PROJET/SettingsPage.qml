import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: settingsRoot
    // 1. AUCUNE ANCRE ICI (C'est le StackView qui donne la taille à cet Item)

    ColumnLayout {
        // 2. IL FAUT REMETTRE L'ANCRE ICI (Pour que la colonne remplisse l'Item)
        anchors.fill: parent
        anchors.margins: 20
        spacing: 25

        // En-tête avec bouton retour
        RowLayout {
            Layout.fillWidth: true
            RoundButton {
                text: "←"
                onClicked: stackView.pop() // Retour au Dashboard
            }
            Text {
                text: "Paramètres Globaux"
                font.pixelSize: 22
                font.bold: true
                Layout.leftMargin: 15
            }
        }

        // Zone de saisie stylisée dans un Rectangle blanc
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 220
            radius: 12
            color: "white"
            border.color: "#EEE"
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Paramètre 1 :
                //Prix a remplacer par un prix fixe défini par le prix du marché actuel
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    Text { text: "Prix de l'électricité (€/kWh) :"; font.bold: true; color: "#555" }
                    // Utilisation de ton composant personnalisé
                    AppTextField {
                        id: costInput
                        text: window.userCostPerKwh.toFixed(2) // Affichera 0.16 au chargement
                        placeholderText: "0.16"
                        inputMethodHints: Qt.ImhFormattedNumbersOnly
                    }
                }

                // Paramètre 2 : Puissance
                // definir la puissance souscrite
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    Text { text: "Puissance de simulation (kW) :"; font.bold: true; color: "#555" }
                    AppTextField {
                        id: powerInput
                        text: window.userMaxPower.toString()
                        placeholderText: "Ex: 12"
                        inputMethodHints: Qt.ImhDigitsOnly
                    }
                }
            }
        }

        Item { Layout.fillHeight: true } // Espaceur pour pousser les boutons vers le bas

        // Boutons d'action
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

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
                    // Logique de sauvegarde : on convertit le texte en nombres
                    // Remplacement de la virgule par un point pour le parseFloat
                    var newCost = parseFloat(costInput.text.replace(",", "."));
                    var newPower = parseInt(powerInput.text);

                    // Vérification sommaire et mise à jour des propriétés globales du Main
                    if (!isNaN(newCost) && newCost > 0) {
                        window.userCostPerKwh = newCost;
                    }
                    if (!isNaN(newPower) && newPower > 0) {
                        window.userMaxPower = newPower;
                    }

                    // Retour automatique au Dashboard
                    stackView.pop();
                }
            }
        }
    }
}
