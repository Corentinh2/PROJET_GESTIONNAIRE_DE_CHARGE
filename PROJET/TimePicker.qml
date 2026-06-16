import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property alias tumblerHeure: tumblerHeure
    property alias tumblerMinute: tumblerMinute

    implicitWidth: 220
    implicitHeight: 120

    Rectangle {
        anchors.fill: parent
        radius: 14
        color: "white"
        border.color: "#EEEEEE"
        border.width: 1

        // Ligne de sélection centrale
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            y: (parent.height / 2) - 22
            width: parent.width - 20
            height: 40
            radius: 8
            color: "#1E88E5"
            opacity: 0.08
        }

        RowLayout {
            anchors.centerIn: parent
            spacing: 0

            // --- Tambour Heures ---
            Tumbler {
                id: tumblerHeure
                model: 24
                width: 80
                implicitHeight: 120
                visibleItemCount: 3
                wrap: true

                delegate: Item {
                    width: tumblerHeure.width
                    height: tumblerHeure.height / tumblerHeure.visibleItemCount

                    Text {
                        anchors.centerIn: parent
                        text: modelData < 10 ? "0" + modelData : "" + modelData
                        font.pixelSize: {
                            if (Tumbler.displacement === 0) { return 22; }
                            return 16;
                        }
                        font.bold: Tumbler.displacement === 0
                        color: {
                            var d = Math.abs(Tumbler.displacement)
                            if (d === 0) { return "#1E88E5"; }
                            return "#B0BEC5";
                        }
                        opacity: {
                            var d = Math.abs(Tumbler.displacement)
                            if (d === 0) { return 1.0; }
                            if (d < 1) { return 0.6; }
                            return 0.3;
                        }
                    }
                }
            }

            // --- Séparateur ---
            Text {
                text: ":"
                font.pixelSize: 24
                font.bold: true
                color: "#263238"
                Layout.alignment: Qt.AlignVCenter
                bottomPadding: 4
            }

            // --- Tambour Minutes ---
            Tumbler {
                id: tumblerMinute
                model: 60
                width: 80
                implicitHeight: 120
                visibleItemCount: 3
                wrap: true

                delegate: Item {
                    width: tumblerMinute.width
                    height: tumblerMinute.height / tumblerMinute.visibleItemCount

                    Text {
                        anchors.centerIn: parent
                        text: modelData < 10 ? "0" + modelData : "" + modelData
                        font.pixelSize: {
                            if (Tumbler.displacement === 0) { return 22; }
                            return 16;
                        }
                        font.bold: Tumbler.displacement === 0
                        color: {
                            var d = Math.abs(Tumbler.displacement)
                            if (d === 0) { return "#1E88E5"; }
                            return "#B0BEC5";
                        }
                        opacity: {
                            var d = Math.abs(Tumbler.displacement)
                            if (d === 0) { return 1.0; }
                            if (d < 1) { return 0.6; }
                            return 0.3;
                        }
                    }
                }
            }
        }
    }
}
