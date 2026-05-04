import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: dashboardRoot

    StackView.onActivated: {
        sessionsModel.clear();
        commEsp.obtenirCalendrier();
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: parent.width
        clip: true

        ColumnLayout {
            width: parent.width - 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 16

            Item { height: 8 }

            Rectangle {
                Layout.fillWidth: true
                height: 80; radius: 16; color: "#E3F2FD"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 18; anchors.rightMargin: 18

                    ColumnLayout {
                        Layout.fillWidth: true; spacing: 2
                        Text { text: window.activeStation; font.bold: true; font.pixelSize: 18; color: "#1565C0" }
                        Text { text: "Supervision active"; font.pixelSize: 13; color: "#42A5F5" }
                    }
                }
            }

            Item { height: 4 }

            Text {
                text: "Navigation"
                font.pixelSize: 12; font.bold: true; color: "#90A4AE"
                font.letterSpacing: 1; Layout.leftMargin: 4
            }

            ColumnLayout {
                Layout.fillWidth: true; spacing: 10

                AppCard {
                    title: "Sessions"
                    subTitle: "Historique et planning"
                    iconChar: "📅"
                    badgeCount: window.activeSessionCount
                    onClicked: stackView.push("SessionsPage.qml")
                }

                AppCard {
                    title: "Statut Borne"
                    subTitle: "Consommation en direct"
                    iconChar: "📊"
                    onClicked: stackView.push("StatusPage.qml")
                }
            }

            Item { height: 8 }

            AppButton {
                text: "Déconnecter la borne"
                isPrimary: false
                Layout.fillWidth: true
                onClicked: {
                    stationsModelSource.clear();
                    sessionsModel.clear();
                    window.activeStation = "";
                    commRaspi.obtenirStation();
                    stackView.replace(null, "BootPage.qml");
                }
            }

            Item { height: 8 }
        }
    }
}
