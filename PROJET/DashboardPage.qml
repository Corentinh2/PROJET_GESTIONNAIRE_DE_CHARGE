import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "logique.js" as Logique

Item {
    id: dashboardRoot
    // 1. L'élément racine n'a AUCUNE ancre, le StackView gère sa taille.

    ScrollView {
        // 2. Le ScrollView s'étire pour remplir l'Item racine
        anchors.fill: parent
        contentWidth: parent.width
        clip: true

        ColumnLayout {
            // On force la colonne à faire la largeur du ScrollView (moins les marges)
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 20

            // Espaceur en haut
            Item { height: 10 }

            // En-tête
            RowLayout {
                Layout.fillWidth: true
                ColumnLayout {
                    Text { text: "Borne : " + window.activeStation; font.bold: true; font.pixelSize: 20; color: "#2979FF" }
                    Text { text: "Supervision active"; font.pixelSize: 14; color: "#888" }
                }
                Item { Layout.fillWidth: true }
                RoundButton {
                    text: "⚙️"; font.pixelSize: 18
                    onClicked: stackView.push(settingsPage)
                }
            }

            AppButton {
                text: "Déconnecter la borne"; isPrimary: false
                Layout.fillWidth: true
                onClicked: stackView.replace(null, bootPage)
            }

            // Section Cartes
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 12

                AppCard {
                    title: "Sessions"; subTitle: "Historique et planning"; iconChar: "📅"
                    onClicked: stackView.push(sessionsPage)
                }
                AppCard {
                    title: "Véhicules"; subTitle: "Gérer ma flotte"; iconChar: "🚗"
                    onClicked: { window.isBookingFlow = false; stackView.push(vehiculesPage); }
                }
                AppCard {
                    title: "Statut Borne"; subTitle: "Consommation live"; iconChar: "📊"
                    onClicked: stackView.push(statusPage)
                }
            }

            Item { Layout.fillHeight: true }
        }
    }
}
