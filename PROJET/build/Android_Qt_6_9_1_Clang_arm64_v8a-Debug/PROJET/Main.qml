import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window
    // --- FORMAT MOBILE ---
    width: 360
    height: 720
    visible: true
    title: "Gestionnaire de charge"
    color: "#F4F6F9"
    font.family: "Segoe UI"

    // --- DONNÉES GLOBALES ET ÉTAT ---

    property string activeStation: ""
    property string selectedVehicle: ""
    property bool isBookingFlow: false
    property string selectedDays: ""
    property string selectedStartTime: ""
    property string selectedEndTime: ""
    property string selectedMileage: ""

    // Paramètres utilisateur
    property double userCostPerKwh: 0.20
    property int userMaxPower: 12

    // Variables de session
    property int sessionDurationSeconds: 0
    property double sessionEnergyKwh: 0.0
    property double sessionCostEuro: 0.0

    // Modèle pour les alertes
    ListModel {
        id: notificationModel
    }

    // Timer de charge globale
    Timer {
        id: globalChargingTimer
        interval: 1000
        repeat: true
        running: (window.activeStation !== "") && (window.getSelectedStationStatus() === "En Charge")

        onTriggered: {
            window.sessionDurationSeconds += 1;

            var powerKw = window.userMaxPower;
            var energyAdded = powerKw / 3600.0;
            window.sessionEnergyKwh += energyAdded;
            window.sessionCostEuro = window.sessionEnergyKwh * window.userCostPerKwh;

            // Alertes de sécurité
            if (powerKw > 20) {
                if (notificationModel.count === 0) {
                    notificationModel.append({
                        "type": "SURTENSION DÉTECTÉE",
                        "message": "Puissance critique dépassée (" + powerKw + " kW)"
                    });
                }
            }

            var rand = Math.random();
            if (rand > 0.99) {
                if (notificationModel.count === 0) {
                    if (powerKw <= 20) {
                        notificationModel.append({
                            "type": "ALERTE SURCHAUFFE",
                            "message": "Température anormale du câble."
                        });
                    }
                }
            }
        }
    }

    // Bases de données (Modèles)
    ListModel { id: sessionsModel }

    ListModel {
        id: vehiclesModel
        ListElement { name: "Tesla Model 3"; year: "2023"; km: "12500" }
        ListElement { name: "Renault Zoe"; year: "2022"; km: "42300" }
        ListElement { name: "Volkswagen ID.4"; year: "2024"; km: "5600" }
    }

    ListModel {
        id: stationsModelSource
        ListElement { name: "Borne A"; kwh: "22 kW"; status: "Disponible" }
        ListElement { name: "Borne B"; kwh: "22 kW"; status: "Disponible" }
        ListElement { name: "Borne C"; kwh: "50 kW"; status: "Disponible" }
    }

    // --- LOGIQUE METIER STRICTE ---

    function getVehicleMileage(vName) {
        var kmVal = 0;
        for (var i = 0; i < vehiclesModel.count; i++) {
            var item = vehiclesModel.get(i);
            if (item.name === vName) {
                kmVal = parseInt(item.km);
            }
        }
        return kmVal;
    }

    function selectActiveStation(stationName) {
        var success = true;
        window.activeStation = stationName;
        stackView.replace(null, dashboardPage);
        return success;
    }

    function addVehicleToModel(name, year, km) {
        var success = false;
        if (name !== "") {
            vehiclesModel.append({ "name": name, "year": year, "km": km });
            success = true;
        }
        return success;
    }

    function removeVehicle(index) {
        var success = true;
        var item = vehiclesModel.get(index);
        if (item.name === window.selectedVehicle) {
            window.selectedVehicle = "";
        }
        vehiclesModel.remove(index);
        return success;
    }

    function startBookingProcess() {
        var success = true;
        window.isBookingFlow = true;
        window.selectedDays = "";
        window.selectedStartTime = "";
        window.selectedEndTime = "";
        stackView.push(schedulePage);
        return success;
    }

    function saveSchedule(daysStr, startStr, endStr) {
        var success = true;
        window.selectedDays = daysStr;
        window.selectedStartTime = startStr;

        // --- INTELLIGENCE POUR LE LENDEMAIN (Heures creuses) ---
        var startHour = parseInt(startStr.substring(0, 2));
        var endHour = parseInt(endStr.substring(0, 2));
        var finalEndStr = endStr;

        if (endHour < startHour) {
            finalEndStr = endStr + " (lendemain)";
        }

        window.selectedEndTime = finalEndStr;
        // -------------------------------------------------------

        var hasVehicle = false;
        if (window.selectedVehicle !== "") {
            hasVehicle = true;
        }

        if (hasVehicle) {
            stackView.push(mileagePage);
        }
        if (!hasVehicle) {
            stackView.push(vehiclesPage);
        }
        return success;
    }

    function selectVehicle(vehicleName) {
        var success = true;
        window.selectedVehicle = vehicleName;
        if (window.isBookingFlow) {
            stackView.push(mileagePage);
        }
        if (!window.isBookingFlow) {
            stackView.pop(null);
        }
        return success;
    }

    function finalizeBooking(km) {
        var found = false;
        window.selectedMileage = km;

        for (var v = 0; v < vehiclesModel.count; v++) {
            var vItem = vehiclesModel.get(v);
            if (vItem.name === window.selectedVehicle) {
                vehiclesModel.setProperty(v, "km", km);
            }
        }

        sessionsModel.append({
            "station": window.activeStation,
            "vehicle": window.selectedVehicle,
            "days": window.selectedDays,
            "start": window.selectedStartTime,
            "end": window.selectedEndTime
        });

        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.activeStation) {
                if (item.status === "Disponible") {
                    stationsModelSource.setProperty(i, "status", "Programmé");
                }
                found = true;
            }
        }

        window.isBookingFlow = false;
        stackView.pop(null);
        return found;
    }

    function deleteSession(index) {
        var success = false;
        if (index >= 0) {
            if (index < sessionsModel.count) {
                sessionsModel.remove(index);
                success = true;
            }
        }

        var remainingSessions = 0;
        for (var k = 0; k < sessionsModel.count; k++) {
            var s = sessionsModel.get(k);
            if (s.station === window.activeStation) {
                remainingSessions = remainingSessions + 1;
            }
        }

        if (remainingSessions === 0) {
            for (var i = 0; i < stationsModelSource.count; i++) {
                var item = stationsModelSource.get(i);
                if (item.name === window.activeStation) {
                    if (item.status !== "En Charge") {
                        stationsModelSource.setProperty(i, "status", "Disponible");
                    }
                }
            }
        }
        return success;
    }

    function toggleForcedMode() {
        var currentState = "Inconnu";
        var actionDone = false;

        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.activeStation) {
                if (!actionDone) {
                    if (item.status === "Disponible") {
                        stationsModelSource.setProperty(i, "status", "En Charge");
                        currentState = "Marche";
                        actionDone = true;
                    }
                }
                if (!actionDone) {
                    if (item.status === "Programmé") {
                        stationsModelSource.setProperty(i, "status", "En Charge");
                        currentState = "Marche";
                        actionDone = true;
                    }
                }
                if (!actionDone) {
                    if (item.status === "En Charge") {
                        var hasSessions = false;
                        for (var k = 0; k < sessionsModel.count; k++) {
                            var s = sessionsModel.get(k);
                            if (s.station === window.activeStation) {
                                hasSessions = true;
                            }
                        }
                        if (hasSessions) {
                            stationsModelSource.setProperty(i, "status", "Programmé");
                        }
                        if (!hasSessions) {
                            stationsModelSource.setProperty(i, "status", "Disponible");
                            window.sessionDurationSeconds = 0;
                            window.sessionEnergyKwh = 0.0;
                            window.sessionCostEuro = 0.0;
                        }
                        currentState = "Arrêt";
                        actionDone = true;
                    }
                }
            }
        }
        return currentState;
    }

    function getSelectedStationStatus() {
        var status = "Inconnu";
        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.activeStation) {
                status = item.status;
            }
        }
        return status;
    }

    // --- NAVIGATION ---
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: bootPage
    }

    // ============================================================
    // PAGE 0 : SÉLECTION DE LA BORNE
    // ============================================================
    Component {
        id: bootPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20

                Item { Layout.fillHeight: true }

                Text {
                    text: "Gestionnaire de charge"
                    font.pixelSize: 26; font.bold: true; color: "#2979FF"
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    text: "Sélectionnez votre borne"
                    font.pixelSize: 16; color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }

                ListView {
                    Layout.fillWidth: true; height: 250; clip: true; spacing: 10
                    model: stationsModelSource
                    delegate: Rectangle {
                        width: parent.width; height: 70; radius: 8; color: "white"; border.color: "#DDD"
                        MouseArea { anchors.fill: parent; onClicked: window.selectActiveStation(name) }
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 15
                            Text { text: "🔌"; font.pixelSize: 20 }
                            Text { text: name; font.bold: true; font.pixelSize: 16 }
                            Item { Layout.fillWidth: true }
                            Text { text: ">"; color: "#2979FF"; font.bold: true; font.pixelSize: 18 }
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }
    }

    // ============================================================
    // PAGE 1 : DASHBOARD
    // ============================================================
    Component {
        id: dashboardPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ScrollView {
                anchors.fill: parent; contentWidth: parent.width
                ColumnLayout {
                    width: parent.width; spacing: 20; anchors.margins: 20

                    RowLayout {
                        Layout.fillWidth: true
                        ColumnLayout {
                            Text { text: "Borne : " + window.activeStation; font.bold: true; font.pixelSize: 20; color: "#2979FF" }
                            Text { text: "Supervision"; font.pixelSize: 14; color: "#888" }
                        }
                        Item { Layout.fillWidth: true }
                        RoundButton {
                            text: "⚙️"
                            font.pixelSize: 18
                            onClicked: stackView.push(settingsPage)
                        }
                    }

                    AppButton {
                        text: "Changer de borne"; isPrimary: false; Layout.fillWidth: true
                        onClicked: stackView.replace(null, bootPage)
                    }

                    // --- ALERTES ---
                    Rectangle {
                        visible: notificationModel.count > 0
                        Layout.fillWidth: true; height: 60; radius: 8; color: "#FFEBEE"; border.color: "#FF5252"; border.width: 1
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 10
                            Text { text: "⚠️"; font.pixelSize: 20 }
                            Text {
                                text: notificationModel.count > 0 ? notificationModel.get(0).message : ""
                                color: "#D32F2F"; font.bold: true; font.pixelSize: 12; Layout.fillWidth: true; wrapMode: Text.WordWrap
                            }
                            AppButton { text: "OK"; onClicked: notificationModel.clear() }
                        }
                    }

                    // --- MENU PRINCIPAL ---
                    ColumnLayout {
                        Layout.fillWidth: true; spacing: 15
                        AppCard { title: "Sessions"; subTitle: "Gérer les charges"; iconChar: "📅"; onClicked: stackView.push(sessionsPage) }
                        AppCard { title: "Véhicules"; subTitle: "Ma flotte"; iconChar: "🚗"; onClicked: { window.isBookingFlow = false; stackView.push(vehiclesPage); } }
                        AppCard { title: "Statut de la borne"; subTitle: "Contrôle direct"; iconChar: "📊"; onClicked: stackView.push(consumptionPage) }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 2 : SESSIONS PROGRAMMÉES
    // ============================================================
    Component {
        id: sessionsPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 15
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton { text: "←"; onClicked: stackView.pop() }
                    Text { text: "Sessions"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
                }

                AppButton { text: "+ Nouvelle Session"; Layout.fillWidth: true; onClicked: window.startBookingProcess() }

                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true; spacing: 10
                    model: sessionsModel
                    delegate: Rectangle {
                        width: parent.width; height: 90; radius: 8; color: "white"; border.color: "#DDD"
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 10
                            ColumnLayout {
                                Layout.fillWidth: true
                                Text { text: "🚗 " + vehicle; font.bold: true; font.pixelSize: 14 }
                                Text { text: "⏱️ " + days; color: "#666"; font.pixelSize: 12 }
                                Text { text: start + " à " + end; color: "#2979FF"; font.bold: true; font.pixelSize: 12 }
                            }
                            AppButton { text: "X"; isPrimary: false; textColor: "red"; onClicked: window.deleteSession(index) }
                        }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 3 : CALENDRIER
    // ============================================================
    Component {
        id: schedulePage
        Item {
            id: schedulePageItem
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            property var tempDaysList: []

            function toggleDay(dayStr) {
                var list = [];
                for (var i = 0; i < tempDaysList.length; i++) { list.push(tempDaysList[i]); }
                var idx = list.indexOf(dayStr);
                if (idx !== -1) { list.splice(idx, 1); }
                if (idx === -1) { list.push(dayStr); }
                tempDaysList = list;
            }

            function isDaySelected(dayStr) {
                var result = false;
                if (tempDaysList.indexOf(dayStr) !== -1) { result = true; }
                return result;
            }

            function getDaysString() {
                var resultStr = tempDaysList.join(", ");
                return resultStr;
            }

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20
                Text { text: "Jours de charge :"; font.bold: true; font.pixelSize: 16 }

                GridLayout {
                    Layout.fillWidth: true; columns: 4; rowSpacing: 10; columnSpacing: 10
                    Repeater {
                        model: ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"]
                        delegate: Rectangle {
                            property bool selected: schedulePageItem.isDaySelected(modelData)
                            Layout.fillWidth: true; height: 50; radius: 25
                            color: selected ? "#2979FF" : "#DDD"
                            Text { anchors.centerIn: parent; text: modelData; color: parent.selected ? "white" : "black"; font.bold: parent.selected }
                            MouseArea { anchors.fill: parent; onClicked: schedulePageItem.toggleDay(modelData) }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true; spacing: 10; Layout.topMargin: 10
                    Text { text: "Heure de début :"; color: "#666" }
                    // Ajout de 22h dans les options
                    ComboBox { id: sTime; Layout.fillWidth: true; model: ["06:00", "08:00", "12:00", "22:00"] }
                    Text { text: "Heure de fin :"; color: "#666" }
                    // Ajout de 4h dans les options
                    ComboBox { id: eTime; Layout.fillWidth: true; model: ["04:00", "06:00", "08:00", "14:00", "18:00"] }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Suivant"; Layout.fillWidth: true
                        enabled: schedulePageItem.tempDaysList.length > 0
                        onClicked: window.saveSchedule(schedulePageItem.getDaysString(), sTime.currentText, eTime.currentText)
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 4 : VÉHICULES
    // ============================================================
    Component {
        id: vehiclesPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 15
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton { text: "←"; onClicked: stackView.pop() }
                    Text { text: "Véhicules"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
                }

                AppButton { text: "+ Ajouter Véhicule"; Layout.fillWidth: true; onClicked: stackView.push(addVehicleFormPage) }

                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10; model: vehiclesModel
                    delegate: Rectangle {
                        width: parent.width; height: 70; radius: 8; color: "white"; border.color: "#DDD"
                        MouseArea { anchors.fill: parent; onClicked: window.selectVehicle(name) }
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 10
                            ColumnLayout {
                                Layout.fillWidth: true
                                Text { text: "🚗 " + name; font.bold: true; font.pixelSize: 16 }
                                Text { text: km + " km"; color: "#666"; font.pixelSize: 12 }
                            }
                            AppButton { text: "X"; isPrimary: false; textColor: "red"; onClicked: window.removeVehicle(index) }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: addVehicleFormPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20
                Text { text: "Nouveau Véhicule"; font.pixelSize: 20; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 160; radius: 12; color: "white"
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 15; spacing: 10
                        Text { text: "Modèle :"; font.bold: true }
                        AppTextField { id: vName; placeholderText: "Ex: Renault Zoe" }
                        Text { text: "Kilométrage actuel :"; font.bold: true }
                        AppTextField { id: vKm; placeholderText: "Ex: 45000"; inputMethodHints: Qt.ImhDigitsOnly }
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Enregistrer"; Layout.fillWidth: true
                        onClicked: { window.addVehicleToModel(vName.text, "2024", vKm.text); stackView.pop(); }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 5 : KILOMÉTRAGE
    // ============================================================
    Component {
        id: mileagePage
        Item {
            id: mileageRoot
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }

            property int currentVehicleKm: window.getVehicleMileage(window.selectedVehicle)

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20
                Text { text: "Validation"; font.pixelSize: 20; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 15; spacing: 10
                        Text { text: "Mise à jour du véhicule :"; color: "#666" }
                        Text { text: window.selectedVehicle; font.bold: true; font.pixelSize: 18; color: "#2979FF" }
                        Text { text: "Ancien relevé : " + mileageRoot.currentVehicleKm + " km"; font.bold: true }

                        AppTextField {
                            id: kmInput
                            placeholderText: "Nouveau kilométrage"
                            inputMethodHints: Qt.ImhDigitsOnly
                        }
                        Text {
                            visible: kmInput.text !== "" && parseInt(kmInput.text) < mileageRoot.currentVehicleKm
                            text: "⚠️ Erreur : Ne peut être inférieur à l'ancien."
                            color: "red"; font.pixelSize: 12; font.bold: true
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Valider"
                        Layout.fillWidth: true
                        enabled: kmInput.text !== "" && parseInt(kmInput.text) >= mileageRoot.currentVehicleKm
                        onClicked: { window.finalizeBooking(kmInput.text); kmInput.text = ""; }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE STATUS & CONTROLE
    // ============================================================
    Component {
        id: consumptionPage
        Item {
            id: consumptionContent
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }

            property string currentVisualState: "Inconnu"

            Timer {
                interval: 500; running: true; repeat: true
                onTriggered: { consumptionContent.currentVisualState = window.getSelectedStationStatus(); }
            }

            function formatTime(totalSeconds) {
                var m = Math.floor(totalSeconds / 60);
                var s = totalSeconds % 60;
                var resM = m.toString();
                var resS = s.toString();
                if (m < 10) { resM = "0" + resM; }
                if (s < 10) { resS = "0" + resS; }
                var finalTime = resM + ":" + resS;
                return finalTime;
            }

            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20
                RowLayout {
                    RoundButton { text: "←"; onClicked: stackView.pop() }
                    Text { text: "Statut Borne"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 80; radius: 8
                    color: consumptionContent.currentVisualState === "En Charge" ? "#FFF3E0" : (consumptionContent.currentVisualState === "Programmé" ? "#E3F2FD" : "#E8F5E9")
                    border.color: consumptionContent.currentVisualState === "En Charge" ? "#FF9100" : (consumptionContent.currentVisualState === "Programmé" ? "#2979FF" : "#00C853")
                    border.width: 2
                    Text {
                        anchors.centerIn: parent
                        text: consumptionContent.currentVisualState.toUpperCase()
                        font.pixelSize: 22; font.bold: true
                        color: consumptionContent.currentVisualState === "En Charge" ? "#FF9100" : (consumptionContent.currentVisualState === "Programmé" ? "#2979FF" : "#00C853")
                    }
                }

                AppButton {
                    Layout.fillWidth: true
                    text: consumptionContent.currentVisualState === "En Charge" ? "STOPPER LA CHARGE" : "FORCER LA CHARGE"
                    background: Rectangle { color: consumptionContent.currentVisualState === "En Charge" ? "red" : "green"; radius: 8 }
                    textColor: "white"
                    onClicked: { window.toggleForcedMode(); }
                }

                Rectangle {
                    visible: consumptionContent.currentVisualState === "En Charge"
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"; border.color: "#DDD"
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 20; spacing: 10
                        Text { text: "Chronomètre :"; color: "#666" }
                        Text { text: consumptionContent.formatTime(window.sessionDurationSeconds); font.pixelSize: 48; font.bold: true; color: "green"; Layout.alignment: Qt.AlignHCenter }

                        RowLayout {
                            Layout.fillWidth: true; Layout.topMargin: 10
                            ColumnLayout {
                                Text { text: "Énergie" }
                                Text { text: window.sessionEnergyKwh.toFixed(3) + " kWh"; font.bold: true; font.pixelSize: 16 }
                            }
                            Item { Layout.fillWidth: true }
                            ColumnLayout {
                                Text { text: "Coût estimé" }
                                Text { text: window.sessionCostEuro.toFixed(2) + " €"; font.bold: true; font.pixelSize: 16; color: "#2979FF" }
                            }
                        }
                    }
                }
                Item { Layout.fillHeight: true }
            }
        }
    }

    // ============================================================
    // PAGE PARAMÈTRES
    // ============================================================
    Component {
        id: settingsPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 20; spacing: 20
                Text { text: "Paramètres"; font.pixelSize: 24; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 15; spacing: 15

                        Text { text: "Prix de l'électricité (€/kWh) :"; font.bold: true; color: "#333" }
                        AppTextField { id: costInput; text: window.userCostPerKwh.toString(); placeholderText: "Ex: 0.20" }

                        Text { text: "Puissance de la borne (kW) :"; font.bold: true; color: "#333" }
                        AppTextField { id: powerInput; text: window.userMaxPower.toString(); placeholderText: "Ex: 12" }
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Sauvegarder"
                        Layout.fillWidth: true
                        onClicked: {
                            var newCost = parseFloat(costInput.text.replace(",", "."));
                            var newPower = parseInt(powerInput.text);
                            if (!isNaN(newCost)) { window.userCostPerKwh = newCost; }
                            if (!isNaN(newPower)) { window.userMaxPower = newPower; }
                            stackView.pop();
                        }
                    }
                }
            }
        }
    }

    // --- COMPOSANTS GÉNÉRIQUES MOBILES ---
    component AppButton : Button {
        property bool isPrimary: true
        property color textColor: isPrimary ? "white" : "black"
        Layout.preferredHeight: 45
        contentItem: Text { text: parent.text; color: textColor; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
        background: Rectangle { color: isPrimary ? "#2979FF" : "#E0E0E0"; radius: 8 }
    }

    component AppTextField : TextField {
        Layout.fillWidth: true; Layout.preferredHeight: 45
        background: Rectangle { color: "#F5F5F5"; radius: 8; border.width: 1; border.color: parent.activeFocus ? "#2979FF" : "#CCC" }
    }

    component AppCard : Rectangle {
        id: cardRoot
        property string title; property string subTitle; property string iconChar; signal clicked()
        Layout.fillWidth: true; height: 90; radius: 12; color: "white"; border.color: "#DDD"; border.width: 1
        MouseArea { anchors.fill: parent; onClicked: cardRoot.clicked() }
        RowLayout {
            anchors.fill: parent; anchors.margins: 15; spacing: 15
            Rectangle {
                width: 50; height: 50; radius: 12; color: "#F5F5F5"
                Text { anchors.centerIn: parent; text: iconChar; font.pixelSize: 24 }
            }
            ColumnLayout {
                Layout.fillWidth: true
                Text { text: title; font.bold: true; font.pixelSize: 16; color: "#333" }
                Text { text: subTitle; color: "#666"; font.pixelSize: 12 }
            }
            Text { text: ">"; color: "#2979FF"; font.bold: true; font.pixelSize: 20 }
        }
    }
}
