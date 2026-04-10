import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: sessionsRoot

    Component.onCompleted: console.log("[SESSIONS] Borne active : " + window.activeStation)

    property int editingIndex: -1
    property int sessionIdASupprimer: -1
    property int activeSessionCount: {
        var count = 0;
        for (var i = 0; i < sessionsModel.count; i++) {
            if (sessionsModel.get(i).station === window.activeStation) count++;
        }
        return count;
    }

    function findIndexInArray(array, value) {
        for (var i = 0; i < array.length; i++) { if (array[i] === value) return i; }
        return 0;
    }

    function showToast(msg) {
        toastText.text = msg;
        toast.opacity = 1;
        toastTimer.restart();
    }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 15; spacing: 14

        // En-tête
        RowLayout {
            Layout.fillWidth: true; spacing: 10
            Rectangle {
                width: 36; height: 36; radius: 18; color: "#F5F5F5"
                Text { anchors.centerIn: parent; text: "←"; font.pixelSize: 16; color: "#546E7A" }
                MouseArea { anchors.fill: parent; onClicked: stackView.pop() }
            }
            Text { text: "Mes Sessions"; font.pixelSize: 20; font.bold: true; color: "#263238" }
            Item { Layout.fillWidth: true }
            Rectangle {
                width: 28; height: 28; radius: 14
                color: sessionsRoot.activeSessionCount > 0 ? "#E3F2FD" : "#F5F5F5"
                Text {
                    anchors.centerIn: parent; text: sessionsRoot.activeSessionCount
                    font.pixelSize: 13; font.bold: true
                    color: sessionsRoot.activeSessionCount > 0 ? "#1E88E5" : "#B0BEC5"
                }
            }
        }

        AppButton {
            text: "+ Programmer une charge"; Layout.fillWidth: true
            onClicked: {
                sessionsRoot.editingIndex = -1;
                window.bookingDays = ""; window.bookingStart = ""; window.bookingEnd = "";
                stackView.push(scheduleStep, { "tempDaysList": [], "initialStart": "08:00", "initialEnd": "17:00" });
            }
        }

        // État vide
        Item {
            Layout.fillWidth: true; Layout.fillHeight: true
            visible: sessionsRoot.activeSessionCount === 0
            ColumnLayout {
                anchors.centerIn: parent; spacing: 12
                Rectangle {
                    width: 64; height: 64; radius: 32; color: "#F5F5F5"; Layout.alignment: Qt.AlignHCenter
                    Text { anchors.centerIn: parent; text: "📅"; font.pixelSize: 28 }
                }
                Text { text: "Aucune session programmée"; font.pixelSize: 15; font.bold: true; color: "#90A4AE"; Layout.alignment: Qt.AlignHCenter }
                Text {
                    text: "Appuyez sur le bouton ci-dessus\npour programmer une charge"
                    font.pixelSize: 12; color: "#B0BEC5"; horizontalAlignment: Text.AlignHCenter; Layout.alignment: Qt.AlignHCenter
                }
            }
        }

        ListView {
            id: sessionList
            Layout.fillWidth: true; Layout.fillHeight: true
            clip: true; spacing: 10
            visible: sessionsRoot.activeSessionCount > 0
            model: sessionsModel

            delegate: Item {
                width: sessionList.width
                property bool isGoodStation: station === window.activeStation
                height: isGoodStation ? 110 : 0; visible: isGoodStation

                Rectangle {
                    width: parent.width; height: 100; radius: 14
                    color: "white"; border.color: "#E3F2FD"; border.width: 2
                    Rectangle { anchors.fill: parent; anchors.topMargin: 3; radius: parent.radius; color: "#000000"; opacity: 0.04; z: -1 }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sessionsRoot.editingIndex = index;
                            stackView.push(scheduleStep, {
                                "tempDaysList": days.split(", "),
                                "initialStart": start,
                                "initialEnd": end.replace(" (lendemain)", "")
                            });
                        }
                    }
                    RowLayout {
                        anchors.fill: parent; anchors.leftMargin: 14; anchors.rightMargin: 10; spacing: 12
                        Rectangle { width: 4; height: 60; radius: 2; color: "#1E88E5"; Layout.alignment: Qt.AlignVCenter }
                        ColumnLayout {
                            Layout.fillWidth: true; spacing: 3
                            Text { text: vehicle; font.bold: true; font.pixelSize: 14; color: "#263238" }
                            Text { text: days; color: "#1E88E5"; font.pixelSize: 12; font.bold: true }
                            RowLayout {
                                spacing: 4
                                Text { text: "🕐"; font.pixelSize: 10 }
                                Text { text: start + " → " + end; color: "#78909C"; font.pixelSize: 11 }
                            }
                        }
                        Rectangle {
                            width: 34; height: 34; radius: 17; color: "#FFEBEE"; Layout.alignment: Qt.AlignVCenter
                            Text { anchors.centerIn: parent; text: "\u2715"; color: "#E53935"; font.pixelSize: 14; font.bold: true }
                            MouseArea {
                                anchors.fill: parent; z: 2
                                onClicked: { sessionsRoot.sessionIdASupprimer = sessionId; confirmPopup.visible = true; }
                            }
                        }
                    }
                }
            }
        }

        Component {
            id: scheduleStep
            Item {
                id: schedulePageItem
                property var tempDaysList: []
                property string initialStart: "08:00"
                property string initialEnd: "17:00"
                property var timeModelData: ["07:00", "08:00", "09:00", "17:00", "18:00", "20:00", "21:00", "22:00", "23:00", "00:00"]

                Component.onCompleted: {
                    sTime.currentIndex = sessionsRoot.findIndexInArray(timeModelData, initialStart);
                    eTime.currentIndex = sessionsRoot.findIndexInArray(timeModelData, initialEnd);
                }

                function toggleDay(dayStr) {
                    var list = tempDaysList.slice();
                    var idx = list.indexOf(dayStr);
                    if (idx !== -1) list.splice(idx, 1); else list.push(dayStr);
                    tempDaysList = list;
                }
                function isDaySelected(dayStr) { return tempDaysList.indexOf(dayStr) !== -1; }
                function getDaysString() { return tempDaysList.join(", "); }

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 20; spacing: 20

                    Text {
                        text: sessionsRoot.editingIndex === -1 ? "Programmer une charge" : "Modifier la charge"
                        font.bold: true; font.pixelSize: 20; color: "#263238"
                    }

                    Rectangle {
                        Layout.fillWidth: true; height: 40; radius: 10
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: "#E3F2FD" }
                            GradientStop { position: 1.0; color: "#BBDEFB" }
                        }
                        Text { anchors.centerIn: parent; text: "🚗  " + window.selectedVehicle; font.bold: true; font.pixelSize: 13; color: "#1565C0" }
                    }

                    GridLayout {
                        Layout.fillWidth: true; columns: 4; rowSpacing: 8; columnSpacing: 8
                        Repeater {
                            model: ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"]
                            delegate: Rectangle {
                                property bool isSelected: schedulePageItem.isDaySelected(modelData)
                                Layout.fillWidth: true; height: 44; radius: 12
                                color: isSelected ? "#1E88E5" : "#F5F5F5"
                                border.color: isSelected ? "#1565C0" : "#E0E0E0"; border.width: 1
                                Text { anchors.centerIn: parent; text: modelData; color: parent.isSelected ? "white" : "#546E7A"; font.bold: parent.isSelected; font.pixelSize: 13 }
                                MouseArea { anchors.fill: parent; onClicked: schedulePageItem.toggleDay(modelData) }
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true; height: 160; radius: 14; color: "white"; border.color: "#EEEEEE"; border.width: 1
                        ColumnLayout {
                            anchors.fill: parent; anchors.margins: 14; spacing: 8
                            Text { text: "Heure de début"; color: "#78909C"; font.pixelSize: 12 }
                            ComboBox { id: sTime; Layout.fillWidth: true; model: schedulePageItem.timeModelData }
                            Text { text: "Heure de fin"; color: "#78909C"; font.pixelSize: 12 }
                            ComboBox { id: eTime; Layout.fillWidth: true; model: schedulePageItem.timeModelData }
                        }
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.fillWidth: true; spacing: 12
                        AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                        AppButton {
                            text: sessionsRoot.editingIndex === -1 ? "Programmer ✓" : "Enregistrer ✓"
                            Layout.fillWidth: true; enabled: schedulePageItem.tempDaysList.length > 0
                            onClicked: {
                                var startStr = sTime.currentText;
                                var endStr = eTime.currentText;
                                window.bookingDays = schedulePageItem.getDaysString();
                                window.bookingStart = startStr;
                                var finalEndStr = parseInt(endStr) < parseInt(startStr) ? endStr + " (lendemain)" : endStr;
                                window.bookingEnd = finalEndStr;
                                if (sessionsRoot.editingIndex !== -1)
                                    commEsp.supprimerCalendrier(sessionsModel.get(sessionsRoot.editingIndex).sessionId);
                                commEsp.ajouterCalendrier(window.bookingDays, window.bookingStart, window.bookingEnd);
                                var msg = sessionsRoot.editingIndex === -1 ? "Session enregistrée ✓" : "Session modifiée ✓";
                                stackView.pop();
                                sessionsRoot.showToast(msg);
                            }
                        }
                    }
                }
            }
        }
    }

    // Popup confirmation suppression
    Rectangle {
        id: confirmPopup
        visible: false; anchors.fill: parent; color: "#AA000000"; z: 10
        Rectangle {
            anchors.centerIn: parent; width: parent.width - 40; radius: 20; color: "white"; height: confirmCol.implicitHeight + 40
            ColumnLayout {
                id: confirmCol; anchors.centerIn: parent; width: parent.width - 40; spacing: 16
                Rectangle { width: 64; height: 64; radius: 32; color: "#FFEBEE"; Layout.alignment: Qt.AlignHCenter; Text { anchors.centerIn: parent; text: "🗑️"; font.pixelSize: 30 } }
                Text { text: "Supprimer la session ?"; font.bold: true; font.pixelSize: 18; color: "#263238"; Layout.alignment: Qt.AlignHCenter }
                Text { text: "Cette action est irréversible."; font.pixelSize: 13; color: "#90A4AE"; horizontalAlignment: Text.AlignHCenter; Layout.fillWidth: true }
                RowLayout {
                    Layout.fillWidth: true; spacing: 12
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: confirmPopup.visible = false }
                    AppButton {
                        text: "Supprimer"; Layout.fillWidth: true
                        onClicked: { commEsp.supprimerCalendrier(sessionsRoot.sessionIdASupprimer); confirmPopup.visible = false; }
                    }
                }
            }
        }
    }

    // Toast
    Rectangle {
        id: toast
        anchors.bottom: parent.bottom; anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        width: toastText.implicitWidth + 40; height: 40; radius: 20
        color: "#323232"; opacity: 0; z: 20
        Behavior on opacity { NumberAnimation { duration: 300 } }
        Text { id: toastText; anchors.centerIn: parent; color: "white"; font.pixelSize: 13 }
        Timer { id: toastTimer; interval: 2000; onTriggered: toast.opacity = 0 }
    }
}
