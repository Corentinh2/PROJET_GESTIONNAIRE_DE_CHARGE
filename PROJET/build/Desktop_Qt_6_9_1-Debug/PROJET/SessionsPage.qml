import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique // Indispensable pour les fonctions JS

Item {
    id: sessionsRoot

    // --- VUE 1 : LA LISTE DES SESSIONS (Page principale de ce fichier) ---
    ColumnLayout {
        anchors.fill: parent // <--- CORRECTION ICI : On remplit la page
        anchors.margins: 15
        spacing: 15

        // En-tête
        RowLayout {
            Layout.fillWidth: true
            RoundButton { text: "←"; onClicked: stackView.pop() }
            Text { text: "Mes Sessions"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
        }

        // Bouton pour lancer le tunnel de programmation
        AppButton {
            text: "+ Programmer une charge"
            Layout.fillWidth: true
            onClicked: {
                // Initialisation des variables globales dans le Main via JS
                Logique.startBookingProcess();
                // On pousse la première étape du tunnel : le calendrier
                stackView.push(scheduleStep);
            }
        }

        // Liste des sessions (ListView)
        ListView {
            id: sessionList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10
            model: sessionsModel // Défini dans Main.qml

            delegate: Rectangle {
                width: sessionList.width
                height: 100
                radius: 10
                color: "white"
                border.color: "#EEE"
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    // Icône d'état (basée sur la station)
                    Rectangle {
                        width: 50; height: 50; radius: 25
                        color: "#E3F2FD" // Bleu clair par défaut
                        Text { anchors.centerIn: parent; text: "⚡"; font.pixelSize: 24 }
                    }

                    // Infos textuelles
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: "🚗 " + vehicle; font.bold: true; font.pixelSize: 16; color: "#333" }
                        Text { text: "🔌 " + station; color: "#666"; font.pixelSize: 12 }
                        Text { text: "📅 " + days + " - de " + start + " à " + end; color: "#2979FF"; font.bold: true; font.pixelSize: 12 }
                    }

                    // Bouton Supprimer
                    AppButton {
                        text: "X"
                        isPrimary: false
                        textColor: "#FF5252"
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        onClicked: Logique.deleteSession(index) // Suppression via JS
                    }
                }
            }
        }
    }

    // =========================================================================
    // --- SOUS-PAGES DU TUNNEL DE RÉSERVATION (Intégrées comme Components) ---
    // =========================================================================

    // --- ÉTAPE 1 : LE CALENDRIER (Jours et Heures) ---
    Component {
        id: scheduleStep
        Item {
            id: schedulePageItem
            // Liste temporaire locale pour stocker les jours cochés
            property var tempDaysList: []

            // Fonctions locales pour gérer la sélection multiple des jours
            function toggleDay(dayStr) {
                var list = [];
                for (var i = 0; i < tempDaysList.length; i++) { list.push(tempDaysList[i]); }
                var idx = list.indexOf(dayStr);
                if (idx !== -1) { list.splice(idx, 1); } // Retire si existe déjà
                else { list.push(dayStr); }  // Ajoute si n'existe pas
                tempDaysList = list; // Mise à jour pour forcer le rafraîchissement QML
            }

            function isDaySelected(dayStr) {
                return tempDaysList.indexOf(dayStr) !== -1;
            }

            function getDaysString() {
                return tempDaysList.join(", ");
            }

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 25

                Text { text: "Étape 1 : Quand charger ?"; font.bold: true; font.pixelSize: 20; color: "#2979FF" }
                Text { text: "Sélectionnez les jours :"; font.bold: true; color: "#555" }

                // Grille pour afficher les pastilles de jours (Repeater)
                GridLayout {
                    Layout.fillWidth: true; columns: 4; rowSpacing: 10; columnSpacing: 10
                    Repeater {
                        model: ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"]
                        delegate: Rectangle {
                            // Propriété calculée dynamiquement
                            property bool isSelected: schedulePageItem.isDaySelected(modelData)

                            Layout.fillWidth: true; height: 50; radius: 25
                            // Changement de couleur si coché
                            color: isSelected ? "#2979FF" : "#E0E0E0"
                            border.color: isSelected ? "#1976D2" : "#CCC"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent;
                                text: modelData;
                                color: parent.isSelected ? "white" : "black";
                                font.bold: parent.isSelected
                            }

                            MouseArea {
                                anchors.fill: parent;
                                onClicked: schedulePageItem.toggleDay(modelData)
                            }
                        }
                    }
                }

                // Section Heures (ComboBox standard)
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 10; Layout.topMargin: 15
                    Text { text: "Heure de début :"; color: "#666" }
                    ComboBox { id: sTime; Layout.fillWidth: true; model: ["08:00", "09:00", "18:00", "20:00", "22:00"] } // Simplifié pour l'exemple
                    Text { text: "Heure de fin :"; color: "#666" }
                    ComboBox { id: eTime; Layout.fillWidth: true; model: ["07:00", "17:00", "21:00", "23:00", "00:00"] }
                }

                Item { Layout.fillHeight: true } // Espaceur

                // Boutons de navigation
                RowLayout {
                    Layout.fillWidth: true; spacing: 15
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Suivant →"
                        Layout.fillWidth: true
                        // Sécurité : désactivé si aucun jour n'est choisi
                        enabled: schedulePageItem.tempDaysList.length > 0
                        onClicked: {
                            // Sauvegarde dans JS et passage auto à l'étape suivante (Véhicule ou KM)
                            Logique.saveSchedule(schedulePageItem.getDaysString(), sTime.currentText, eTime.currentText);
                            // NOTE : Le JS va gérer le push automatique vers mileageStep ou véhiculesPage selon le cas
                        }
                    }
                }
            }
        }
    }
}
