import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: cardRoot

    property string title
    property string subTitle
    property string iconChar

    signal clicked()

    Layout.fillWidth: true
    height: 80
    radius: 14
    color: "white"
    border.color: "#EEEEEE"
    border.width: 1

    // Ombre douce
    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 3
        radius: parent.radius
        color: "#000000"
        opacity: 0.04
        z: -1
    }

    // Effet au clic
    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: "#000000"
        opacity: ma.pressed ? 0.03 : 0
    }

    MouseArea {
        id: ma
        anchors.fill: parent
        onClicked: cardRoot.clicked()
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        spacing: 14

        // Icône dans un cercle coloré
        Rectangle {
            width: 46
            height: 46
            radius: 23
            color: "#E3F2FD"
            Layout.alignment: Qt.AlignVCenter

            Text {
                anchors.centerIn: parent
                text: iconChar
                font.pixelSize: 22
            }
        }

        // Textes
        ColumnLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            spacing: 3

            Text {
                text: title
                font.bold: true
                font.pixelSize: 15
                color: "#263238"
            }

            Text {
                text: subTitle
                color: "#78909C"
                font.pixelSize: 12
            }
        }

        // Chevron
        Text {
            text: "›"
            color: "#1E88E5"
            font.pixelSize: 24
            font.bold: false
            Layout.alignment: Qt.AlignVCenter
        }
    }
}
