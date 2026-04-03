import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control

    property bool isPrimary: true
    property color textColor: isPrimary ? "#FFFFFF" : "#37474F"

    contentItem: Text {
        text: control.text
        color: control.enabled ? control.textColor : "#9E9E9E"
        font.pixelSize: 14
        font.bold: true
        font.letterSpacing: 0.5
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitHeight: 48
        radius: 10

        color: {
            if (!control.enabled) {
                return "#E0E0E0";
            }
            if (isPrimary) {
                return control.pressed ? "#1565C0" : "#1E88E5";
            }
            return control.pressed ? "#E0E0E0" : "#F5F5F5";
        }

        border.width: isPrimary ? 0 : 1
        border.color: "#E0E0E0"

        // Ombre douce sous le bouton primaire
        Rectangle {
            visible: isPrimary && control.enabled
            anchors.fill: parent
            anchors.topMargin: 2
            radius: parent.radius
            color: "#1E88E5"
            opacity: 0.2
            z: -1
        }
    }
}
