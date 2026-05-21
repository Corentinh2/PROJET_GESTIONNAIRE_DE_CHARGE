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
        color: {
            if (control.activeFocus) { return "#FFFFFF"; }
            return "#F5F7FA";
        }
        radius: 10
        border.width: {
            if (control.activeFocus) { return 2; }
            return 1;
        }
        border.color: {
            if (control.activeFocus) { return "#1E88E5"; }
            return "#E0E0E0";
        }
    }
}
