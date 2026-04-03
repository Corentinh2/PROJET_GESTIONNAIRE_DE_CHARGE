import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control

    font.pixelSize: 14
    color: "#263238"
    placeholderTextColor: "#B0BEC5"
    leftPadding: 14
    rightPadding: 14

    background: Rectangle {
        implicitHeight: 48
        color: control.activeFocus ? "#FFFFFF" : "#F5F7FA"
        radius: 10
        border.width: control.activeFocus ? 2 : 1
        border.color: control.activeFocus ? "#1E88E5" : "#E0E0E0"
    }
}
