import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control
    background: Rectangle {
        implicitHeight: 45
        color: "#F5F5F5"
        radius: 8
        border.width: 1
        border.color: control.activeFocus ? "#2979FF" : "#CCC"
    }
}
