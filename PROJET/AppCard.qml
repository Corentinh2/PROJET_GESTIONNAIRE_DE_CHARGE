import QtQuick 2.15
import QtQuick.Layouts 1.15 // <--- INDISPENSABLE ici

Rectangle {
    id: cardRoot
    property string title
    property string subTitle
    property string iconChar
    signal clicked()

    // On force la taille par défaut
    Layout.fillWidth: true
    height: 90
    radius: 12
    color: "white"
    border.color: "#DDD"
    border.width: 1

    MouseArea {
        anchors.fill: parent
        onClicked: cardRoot.clicked()
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // 1. L'icône
        Rectangle {
            width: 50; height: 50; radius: 12; color: "#F5F5F5"
            Layout.alignment: Qt.AlignVCenter
            Text { anchors.centerIn: parent; text: iconChar; font.pixelSize: 24 }
        }

        // 2. Les textes (On leur dit de prendre toute la place restante)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 2
            Text {
                text: title
                font.bold: true
                font.pixelSize: 16
                color: "#333"
            }
            Text {
                text: subTitle
                color: "#666"
                font.pixelSize: 12
            }
        }

        // 3. La flèche
        Text {
            text: ">"
            color: "#2979FF"
            font.bold: true
            font.pixelSize: 20
            Layout.alignment: Qt.AlignVCenter
        }
    }
}
