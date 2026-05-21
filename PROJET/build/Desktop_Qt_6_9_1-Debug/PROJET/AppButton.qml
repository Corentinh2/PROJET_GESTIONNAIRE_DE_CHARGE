import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control

    property bool isPrimary: true

    property color textColor: {
        if (isPrimary) { return "#FFFFFF"; }
        return "#37474F";
    }

    contentItem: Text {
        text: control.text
        color: {
            if (control.enabled) { return control.textColor; }
            return "#9E9E9E";
        }
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
            if (!control.enabled) { return "#E0E0E0"; }
            if (isPrimary) {
                if (control.pressed) { return "#1565C0"; }
                return "#1E88E5";
            }
            if (control.pressed) { return "#E0E0E0"; }
            return "#F5F5F5";
        }

        border.width: {
            if (isPrimary) { return 0; }
            return 1;
        }
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
