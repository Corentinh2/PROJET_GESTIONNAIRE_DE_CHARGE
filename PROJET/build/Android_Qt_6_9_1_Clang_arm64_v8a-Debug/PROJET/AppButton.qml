import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: control
    property bool isPrimary: true
    property color textColor: isPrimary ? "white" : "black"

    contentItem: Text {
        text: control.text
        color: control.textColor
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitHeight: 45
        color: isPrimary ? "#2979FF" : "#E0E0E0"
        radius: 8
    }
}
