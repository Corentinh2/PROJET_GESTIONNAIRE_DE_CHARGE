import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: sessionsRoot

    Component.onCompleted: {
        console.log("[SESSIONS] Borne active : " + window.activeStation);
        sessionsModel.clear();
        commEsp.obtenirCalendrier();
    }

    property int editingIndex: -1

    function findIndexInArray(array, value) {
        var indexTrouve = 0;
        for (var i = 0; i < array.length; i++) {
            if (array[i] === value) {
                indexTrouve = i;
            }
        }
        return indexTrouve;
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 14

        // En-tête
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                width: 36; height: 36; radius: 18
                color: "#F5F5F5"
                Text { anchors.centerIn: parent; text: "←"; font.pixelSize: 16; color: "#546E7A" }
                MouseArea { anchors.fill: parent; onClicked: stackView.pop() }
            }

            Text {
                text: "Mes Sessions"
                font.pixelSize: 20
                font.bold: true
                color: "#263238"
            }
        }

        AppButton {
            text: "+ Programmer une charge"
            Layout.fillWidth: true
            onClicked: {
                sessionsRoot.editingIndex = -1;
                window.bookingDays = "";
                window.bookingStart = "";
                window.bookingEnd = "";
                stackView.push(scheduleStep, {
                                   "tempDaysList": [],
                                   "initialStart": "08:00",
                                   "initialEnd": "17:00"
                               });
            }
        }

        ListView {
            id: sessionList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 10
            model: sessionsModel

            delegate: Item {
                width: sessionList.width
                property bool isGoodStation: station === window.activeStation
                height: isGoodStation ? 110 : 0
                visible: isGoodStation

                Rectangle {
                    width: parent.width
                    height: 100
                    radius: 14
                    color: "white"
                    border.color: "#EEEEEE"
                    border.width: 1

                    Rectangle {
                        anchors.fill: parent
                        anchors.topMargin: 3
                        radius: parent.radius
                        color: "#000000"
                        opacity: 0.04
                        z: -1
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            sessionsRoot.editingIndex = index;
                            var cleanEnd = end.replace(" (lendemain)", "");
                            stackView.push(scheduleStep, {
                                               "tempDaysList": days.split(", "),
                                               "initialStart": start,
                                               "initialEnd": cleanEnd
                                           });
                        }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 14
                        anchors.rightMargin: 10
                        spacing: 12

                        Rectangle {
                            width: 4; height: 60; radius: 2
                            color: "#1E88E5"
                            Layout.alignment: Qt.AlignVCenter
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 3

                            Text {
                                text: vehicle
                                font.bold: true
                                font.pixelSize: 14
                                color: "#263238"
                            }

                            Text {
                                text: days
                                color: "#1E88E5"
                                font.pixelSize: 12
                                font.bold: true
                            }

                            Text {
                                text: start + " → " + end
                                color: "#78909C"
                                font.pixelSize: 11
                            }
                        }

                        // Bouton Supprimer — utilise l'ID
                        Rectangle {
                            width: 34; height: 34; radius: 17
                            color: "#FFEBEE"
                            Layout.alignment: Qt.AlignVCenter

                            Text {
                                anchors.centerIn: parent
                                text: "✕"
                                color: "#E53935"
                                font.pixelSize: 14
                                font.bold: true
                            }

                            MouseArea {
                                anchors.fill: parent
                                z: 2
                                onClicked: {
                                    // Suppression par ID
                                    commEsp.supprimerCalendrier(sessionId);
                                    sessionsModel.remove(index);
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
                        var list = [];
                        for (var i = 0; i < tempDaysList.length; i++) { list.push(tempDaysList[i]); }
                        var idx = list.indexOf(dayStr);
                        if (idx !== -1) { list.splice(idx, 1); }
                        if (idx === -1) { list.push(dayStr); }
                        tempDaysList = list;
                    }

                    function isDaySelected(dayStr) {
                        var trouve = false;
                        if (tempDaysList.indexOf(dayStr) !== -1) { trouve = true; }
                        return trouve;
                    }

                    function getDaysString() { return tempDaysList.join(", "); }

                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 20; spacing: 20

                        Text {
                            text: sessionsRoot.editingIndex === -1 ? "Programmer une charge" : "Modifier la charge"
                            font.bold: true; font.pixelSize: 20; color: "#263238"
                        }

                        Rectangle {
                            Layout.fillWidth: true; height: 40; radius: 10; color: "#E3F2FD"
                            Text {
                                anchors.centerIn: parent
                                text: "🚗  " + window.selectedVehicle
                                font.bold: true; font.pixelSize: 13; color: "#1565C0"
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true; columns: 4; rowSpacing: 8; columnSpacing: 8
                            Repeater {
                                model: ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"]
                                delegate: Rectangle {
                                    property bool isSelected: schedulePageItem.isDaySelected(modelData)
                                    Layout.fillWidth: true; height: 44; radius: 12
                                    color: isSelected ? "#1E88E5" : "#F5F5F5"
                                    border.color: isSelected ? "#1565C0" : "#E0E0E0"
                                    border.width: 1

                                    Text {
                                        anchors.centerIn: parent; text: modelData
                                        color: parent.isSelected ? "white" : "#546E7A"
                                        font.bold: parent.isSelected; font.pixelSize: 13
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: schedulePageItem.toggleDay(modelData)
                                    }
                                }
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true; height: 120; radius: 14
                            color: "white"; border.color: "#EEEEEE"; border.width: 1

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

                            AppButton {
                                text: "Annuler"; isPrimary: false; Layout.fillWidth: true
                                onClicked: stackView.pop()
                            }

                            AppButton {
                                text: sessionsRoot.editingIndex === -1 ? "Programmer ✓" : "Enregistrer ✓"
                                Layout.fillWidth: true
                                enabled: schedulePageItem.tempDaysList.length > 0

                                onClicked: {
                                    var startStr = sTime.currentText;
                                    var endStr = eTime.currentText;
                                    window.bookingDays = schedulePageItem.getDaysString();
                                    window.bookingStart = startStr;
                                    var startHour = parseInt(startStr.substring(0, 2));
                                    var endHour = parseInt(endStr.substring(0, 2));
                                    var finalEndStr = endStr;
                                    if (endHour < startHour) {
                                        finalEndStr = endStr + " (lendemain)";
                                    }
                                    window.bookingEnd = finalEndStr;

                                    // Mode modification : supprimer l'ancien par ID
                                    if (sessionsRoot.editingIndex !== -1) {
                                        var oldSession = sessionsModel.get(sessionsRoot.editingIndex);
                                        commEsp.supprimerCalendrier(oldSession.sessionId);
                                        sessionsModel.remove(sessionsRoot.editingIndex);
                                    }

                                    // Ajout du nouveau calendrier
                                    commEsp.ajouterCalendrier(
                                                window.bookingDays,
                                                window.bookingStart,
                                                window.bookingEnd
                                                );

                                    stackView.pop();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
