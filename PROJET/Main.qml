import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: "EV Charge Manager"
    color: "#F4F6F9"
    font.family: "Segoe UI"

    // --- DONNÉES GLOBALES ET ÉTAT ---

    // Variables tampons pour la création de session
    property string selectedStation: ""
    property string selectedVehicle: ""
    property bool isBookingFlow: false

    property string selectedDays: ""
    property string selectedStartTime: ""
    property string selectedEndTime: ""
    property string selectedMileage: ""

    // On considère qu'il y a une session active si le modèle n'est pas vide
    property bool hasActiveSession: sessionsModel.count > 0

    // --- Paramètres Utilisateur ---
    property double userCostPerKwh: 0.20
    property int userMaxPower: 12

    // --- Cerveau de la Session (Global) ---
    // Ces variables stockent l'état de la session active visible
    property int sessionDurationSeconds: 0
    property double sessionEnergyKwh: 0.0
    property double sessionCostEuro: 0.0

    // --- Timer Global ---
    Timer {
        id: globalChargingTimer
        interval: 1000
        repeat: true
        // Tourne si on a des sessions ET que la station ACTUELLEMENT SÉLECTIONNÉE est en charge
        running: window.hasActiveSession && (window.getSelectedStationStatus() === "En Charge")

        onTriggered: {
            window.sessionDurationSeconds += 1;
            // Formule : Puissance (kW) / 3600 = kWh par seconde
            var powerKw = window.userMaxPower;
            var energyAdded = powerKw / 3600.0;
            window.sessionEnergyKwh += energyAdded;
            window.sessionCostEuro = window.sessionEnergyKwh * window.userCostPerKwh;
        }
    }

    // --- MODÈLE DES SESSIONS PROGRAMMÉES (POUR LE CARROUSEL) ---
    ListModel {
        id: sessionsModel
        // Structure: { station, vehicle, days, start, end }
    }

    // Modèle Source Véhicules
    ListModel {
        id: vehiclesModel
        ListElement { name: "Tesla Model 3"; year: "2023"; charge: 0.85; batt: "75 kWh"; km: "12500" }
        ListElement { name: "Renault Zoe"; year: "2022"; charge: 0.42; batt: "52 kWh"; km: "42300" }
        ListElement { name: "Volkswagen ID.4"; year: "2024"; charge: 0.68; batt: "77 kWh"; km: "5600" }
    }

    // Modèle Source Bornes
    ListModel {
        id: stationsModelSource
        ListElement { name: "Borne A"; loc: "Niveau 1"; dist: "50m"; kwh: "22 kW"; type: "Type 2"; status: "Disponible" }
        ListElement { name: "Borne B"; loc: "Niveau 1"; dist: "55m"; kwh: "22 kW"; type: "Type 2"; status: "Occupée" }
        ListElement { name: "Borne C"; loc: "Extérieur"; dist: "120m"; kwh: "50 kW"; type: "CCS"; status: "Disponible" }
        ListElement { name: "Borne D"; loc: "Extérieur"; dist: "125m"; kwh: "50 kW"; type: "CCS"; status: "Disponible" }
    }

    ListModel {
        id: stationsModelDisplay
        Component.onCompleted: window.updateStationFilter(false)
    }

    property bool showOnlyAvailable: false

    // --- LOGIQUE METIER ---

    // Mise à jour des variables globales quand on change de slide dans le carrousel
    function syncGlobalsWithSession(index) {
        if (index >= 0 && index < sessionsModel.count) {
            var item = sessionsModel.get(index);
            window.selectedStation = item.station;
            window.selectedVehicle = item.vehicle;
            window.selectedDays = item.days;
            window.selectedStartTime = item.start;
            window.selectedEndTime = item.end;
        } else {
            // Plus de session
            window.selectedStation = "";
            window.selectedVehicle = "";
        }
    }

    function addStationToModel(name, loc, power) {
        var success = false;
        if (name !== "") {
            stationsModelSource.append({
                "name": name, "loc": loc, "dist": "N/A", "kwh": power + " kW", "type": "Type 2", "status": "Disponible"
            });
            window.updateStationFilter(window.showOnlyAvailable);
            success = true;
        }
        return success;
    }

    function removeStation(stationName) {
        var removed = false;
        for (var i = stationsModelSource.count - 1; i >= 0; i--) {
            var item = stationsModelSource.get(i);
            if (item.name === stationName) {
                // Supprimer aussi la session associée si elle existe
                for(var k = sessionsModel.count - 1; k >=0; k--) {
                    if (sessionsModel.get(k).station === stationName) {
                        window.deleteSession(k);
                    }
                }
                stationsModelSource.remove(i);
                removed = true;
            }
        }
        window.updateStationFilter(window.showOnlyAvailable);
        return removed;
    }

    function addVehicleToModel(name, year, km) {
        var success = false;
        if (name !== "") {
            vehiclesModel.append({ "name": name, "year": year, "batt": "N/A", "km": km });
            success = true;
        }
        return success;
    }

    function removeVehicle(index) {
        var item = vehiclesModel.get(index);
        // Si c'est le véhicule sélectionné hors résa, on le vide
        if (item.name === window.selectedVehicle) {
            window.selectedVehicle = "";
        }
        vehiclesModel.remove(index);
        return true;
    }

    function startBookingProcess(stationName) {
        window.selectedStation = stationName; // Temp pour la création
        window.isBookingFlow = true;
        window.selectedDays = "";
        window.selectedStartTime = "";
        window.selectedEndTime = "";
        stackView.push(schedulePage);
        return true;
    }

    function saveSchedule(daysStr, startStr, endStr) {
        window.selectedDays = daysStr;
        window.selectedStartTime = startStr;
        window.selectedEndTime = endStr;
        // Si un véhicule est déjà choisi (hors résa), on saute l'étape véhicule
        if (window.selectedVehicle !== "") {
            stackView.push(mileagePage);
        } else {
            stackView.push(vehiclesPage);
        }
        return true;
    }

    function selectVehicle(vehicleName) {
        window.selectedVehicle = vehicleName;
        if (window.isBookingFlow) {
            stackView.push(mileagePage);
        } else {
            stackView.pop(null);
        }
        return true;
    }

    function finalizeBooking(km) {
        window.selectedMileage = km;

        // Ajout au carrousel
        sessionsModel.append({
            "station": window.selectedStation,
            "vehicle": window.selectedVehicle,
            "days": window.selectedDays,
            "start": window.selectedStartTime,
            "end": window.selectedEndTime
        });

        // Mise à jour du statut de la borne
        var found = false;
        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.selectedStation) {
                stationsModelSource.setProperty(i, "status", "Programmé");
                found = true;
            }
        }

        window.updateStationFilter(window.showOnlyAvailable);
        window.isBookingFlow = false;
        stackView.pop(null);
        return found;
    }

    // Supprime une session spécifique par index
    function deleteSession(index) {
        if (index >= 0 && index < sessionsModel.count) {
            var item = sessionsModel.get(index);
            var stationName = item.station;

            // Libérer la borne correspondante
            for (var i = 0; i < stationsModelSource.count; i++) {
                var sItem = stationsModelSource.get(i);
                if (sItem.name === stationName) {
                    stationsModelSource.setProperty(i, "status", "Disponible");
                }
            }

            // Retirer du modèle
            sessionsModel.remove(index);

            // Reset compteurs si plus aucune session
            if (sessionsModel.count === 0) {
                window.sessionDurationSeconds = 0;
                window.sessionEnergyKwh = 0.0;
                window.sessionCostEuro = 0.0;
                window.selectedStation = ""; // Clean UI
                window.selectedVehicle = "";
            }

            window.updateStationFilter(window.showOnlyAvailable);
        }
        return true;
    }

    function toggleForcedMode() {
        var currentState = "Inconnu";
        // On agit sur la borne actuellement sélectionnée (celle visible dans le carrousel)
        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.selectedStation) {
                if (item.status === "Programmé") {
                    stationsModelSource.setProperty(i, "status", "En Charge");
                    currentState = "Marche";
                } else {
                    if (item.status === "En Charge") {
                        stationsModelSource.setProperty(i, "status", "Programmé");
                        currentState = "Arrêt";
                    }
                }
            }
        }
        window.updateStationFilter(window.showOnlyAvailable);
        return currentState;
    }

    function getSelectedStationStatus() {
        var status = "Aucun";
        if (window.selectedStation !== "") {
            for (var i = 0; i < stationsModelSource.count; i++) {
                var item = stationsModelSource.get(i);
                if (item.name === window.selectedStation) {
                    status = item.status;
                }
            }
        }
        return status;
    }

    function updateStationFilter(onlyAvailable) {
        stationsModelDisplay.clear();
        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            var shouldAdd = true;
            if (onlyAvailable) {
                if (item.status !== "Disponible") { shouldAdd = false; }
            }
            if (shouldAdd) { stationsModelDisplay.append(item); }
        }
        return true;
    }

    function getStatusColor(status) {
        var col = "#888";
        if (status === "Disponible") { col = "#00C853"; }
        else {
            if (status === "Occupée") { col = "#FF5252"; }
            else {
                if (status === "Programmé") { col = "#2979FF"; }
                else { if (status === "En Charge") { col = "#FF9100"; } }
            }
        }
        return col;
    }

    function getStatusBgColor(status) {
        var col = "#EEE";
        if (status === "Disponible") { col = "#E8F5E9"; }
        else {
            if (status === "Occupée") { col = "#FFEBEE"; }
            else {
                if (status === "Programmé") { col = "#E3F2FD"; }
                else { if (status === "En Charge") { col = "#FFF3E0"; } }
            }
        }
        return col;
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: dashboardPage
        pushEnter: Transition { PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } NumberAnimation { property: "x"; from: 100; to: 0; duration: 200; easing.type: Easing.OutCubic } }
        pushExit: Transition { PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } }
        popEnter: Transition { PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } }
        popExit: Transition { PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } NumberAnimation { property: "x"; from: 0; to: 100; duration: 200; easing.type: Easing.InCubic } }
    }

    // ============================================================
    // PAGE 1 : DASHBOARD (AVEC CARROUSEL CORRIGÉ)
    // ============================================================
    Component {
        id: dashboardPage
        Item {
            ScrollView {
                anchors.fill: parent; contentWidth: parent.width
                ColumnLayout {
                    width: parent.width; spacing: 24; anchors.margins: 40
                    RowLayout {
                        Layout.fillWidth: true; Layout.margins: 40; Layout.topMargin: 40
                        Rectangle { width: 48; height: 48; radius: 12; color: "#00C853"; Text { anchors.centerIn: parent; text: "⚡"; color: "white"; font.pixelSize: 24 } }
                        ColumnLayout {
                            Text { text: "EV Charge Manager"; font.bold: true; font.pixelSize: 18; color: "#333" }
                            Text { text: "Gérez vos recharges facilement"; font.pixelSize: 14; color: "#888" }
                        }
                        Item { Layout.fillWidth: true }

                        // Si une voiture est choisie hors session, on l'affiche
                        Rectangle {
                            visible: window.selectedVehicle !== "" && sessionsModel.count === 0
                            width: 200; height: 40; radius: 20; color: "#EDE7F6"
                            RowLayout {
                                anchors.centerIn: parent
                                Text { text: "🚗"; font.pixelSize: 16 }
                                Text { text: window.selectedVehicle; font.bold: true; color: "#673AB7" }
                            }
                        }

                        RowLayout {
                            spacing: 10
                            RoundButton { text: "⚙️"; font.pixelSize: 18; onClicked: stackView.push(settingsPage) }
                            AppButton { text: "Déconnexion"; isPrimary: false; textColor: "#FF5252"; onClicked: print("Logout") }
                        }
                    }

                    // --- CARROUSEL DES SESSIONS ---
                    ColumnLayout {
                        visible: window.hasActiveSession
                        Layout.fillWidth: true; Layout.leftMargin: 40; Layout.rightMargin: 40
                        spacing: 10

                        SwipeView {
                            id: sessionSwipe
                            Layout.fillWidth: true
                            height: 160
                            clip: true

                            // Synchroniser les globales quand on swipe
                            onCurrentIndexChanged: {
                                window.syncGlobalsWithSession(currentIndex);
                            }

                            // Init au chargement
                            Component.onCompleted: {
                                if (sessionsModel.count > 0) window.syncGlobalsWithSession(0);
                            }

                            // CORRECTION MAJEURE ICI : Repeater obligatoire pour SwipeView dynamique
                            Repeater {
                                model: sessionsModel
                                delegate: Rectangle {
                                    // Utilisation de la largeur du SwipeView parent
                                    width: sessionSwipe.width
                                    height: 160
                                    radius: 12; color: "#E3F2FD"
                                    border.color: "#2979FF"; border.width: 1

                                    RowLayout {
                                        anchors.fill: parent; anchors.margins: 20
                                        Rectangle { width: 50; height: 50; radius: 25; color: "#2979FF"; Text { anchors.centerIn: parent; text: "ℹ️"; font.pixelSize: 24 } }
                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            // Les variables 'station', 'vehicle', etc. viennent du modèle
                                            Text { text: "Session Programmée " + (index + 1); font.bold: true; color: "#1565C0" }
                                            Text {
                                                text: "Borne: " + station + "\n" +
                                                      "Véhicule: " + vehicle + "\n" +
                                                      "Jours: " + days + "\n" +
                                                      "Horaire: " + start + " à " + end
                                                color: "#555"
                                            }
                                        }

                                        AppButton {
                                            text: "Supprimer"; isPrimary: false; textColor: "#D32F2F"
                                            onClicked: window.deleteSession(index)
                                        }
                                    }
                                }
                            }
                        }

                        // Indicateur de page
                        PageIndicator {
                            Layout.alignment: Qt.AlignHCenter
                            count: sessionSwipe.count
                            currentIndex: sessionSwipe.currentIndex
                            visible: sessionSwipe.count > 1
                        }
                    }

                    ColumnLayout { Layout.leftMargin: 40
                        Text { text: "Bienvenue, Utilisateur !"; font.pixelSize: 32; font.bold: true; color: "#1a1a1a" }
                    }

                    GridLayout {
                        columns: 3; Layout.fillWidth: true; Layout.leftMargin: 40; Layout.rightMargin: 40; columnSpacing: 20; rowSpacing: 20
                        AppCard {
                            iconChar: "🔌"; iconBg: "#2979FF"; title: "Bornes"; subTitle: "Programmer une charge"; actionText: "Démarrer >"
                            onClicked: { stackView.push(stationsPage); }
                        }
                        AppCard {
                            iconChar: "🚗"; iconBg: "#D500F9"; title: "Mes Véhicules"; subTitle: "Gérer la flotte"; actionText: "Accéder >"
                            onClicked: { window.isBookingFlow = false; stackView.push(vehiclesPage); }
                        }
                        AppCard {
                            iconChar: "📊"; iconBg: "#00C853"; title: "Contrôle & Status"; subTitle: "Marche forcée et stats"; actionText: "Accéder >"
                            onClicked: {
                                // On sync avant d'y aller pour être sûr
                                window.syncGlobalsWithSession(sessionSwipe.currentIndex);
                                stackView.push(consumptionPage);
                            }
                        }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 2 : LISTE BORNES
    // ============================================================
    Component {
        id: stationsPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 40; spacing: 20
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton { text: "←"; onClicked: stackView.pop(); font.pixelSize: 18 }
                    Text { text: "Bornes de Recharge"; font.pixelSize: 28; font.bold: true; color: "#333"; Layout.leftMargin: 10 }
                    Item { Layout.fillWidth: true }
                    AppButton { text: "+ Ajouter"; onClicked: stackView.push(addStationPage) }
                }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true; spacing: 15
                    model: stationsModelDisplay
                    delegate: Rectangle {
                        width: parent.width; height: 120; radius: 12; color: "white"
                        border.color: mouseAreaSt.containsMouse ? "#2979FF" : "#EEF0F2"
                        border.width: mouseAreaSt.containsMouse ? 2 : 1
                        MouseArea {
                            id: mouseAreaSt
                            anchors.fill: parent; hoverEnabled: true; cursorShape: (status === "Disponible") ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: { if (status === "Disponible") { window.startBookingProcess(name); } }
                        }
                        Rectangle {
                            width: 6; height: parent.height; color: window.getStatusColor(status)
                            radius: 12; Rectangle { width: 3; height: parent.height; color: parent.color; anchors.right: parent.right }
                        }
                        Item {
                            anchors.fill: parent; anchors.margins: 15; anchors.leftMargin: 20
                            RowLayout {
                                anchors.top: parent.top; width: parent.width
                                Rectangle { width: 48; height: 48; radius: 12; color: window.getStatusBgColor(status); Text { anchors.centerIn: parent; text: "⚡"; font.pixelSize: 24; color: window.getStatusColor(status) } }
                                ColumnLayout {
                                    Text { text: name; font.bold: true; font.pixelSize: 16; color: "#333" }
                                    Text { text: "📍 " + loc + " • " + kwh; color: "#888"; font.pixelSize: 13 }
                                }
                                Item { Layout.fillWidth: true }
                                RoundButton { text: "🗑️"; font.pixelSize: 16; flat: true; onClicked: window.removeStation(name) }
                            }
                            RowLayout {
                                anchors.bottom: parent.bottom; anchors.right: parent.right
                                Text { visible: status === "Disponible"; text: "Sélectionner >"; color: "#2979FF"; font.bold: true }
                                Text { visible: status !== "Disponible"; text: status; color: window.getStatusColor(status); font.bold: true }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: addStationPage
        Item {
            Rectangle { anchors.fill: parent; color: "white" }
            ColumnLayout {
                anchors.centerIn: parent; width: 400; spacing: 20
                Text { text: "Ajouter une Borne"; font.pixelSize: 24; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }
                AppTextField { id: sName; placeholderText: "Nom (ex: Borne E)" }
                AppTextField { id: sLoc; placeholderText: "Emplacement (ex: Niveau 2)" }
                AppTextField { id: sPower; placeholderText: "Puissance (ex: 22)" }
                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 20
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Enregistrer"; Layout.fillWidth: true
                        onClicked: {
                            var added = window.addStationToModel(sName.text, sLoc.text, sPower.text);
                            if (added) { sName.text = ""; sLoc.text = ""; sPower.text = ""; stackView.pop(); }
                        }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 3 : CALENDRIER AVANCÉ
    // ============================================================
    Component {
        id: schedulePage
        Item {
            id: schedulePageItem
            Rectangle { anchors.fill: parent; color: "white" }
            property var tempDaysList: []
            function toggleDay(dayStr) {
                var list = []; for (var i = 0; i < tempDaysList.length; i++) { list.push(tempDaysList[i]); }
                var idx = list.indexOf(dayStr);
                if (idx !== -1) { list.splice(idx, 1); } else { list.push(dayStr); }
                tempDaysList = list;
            }
            function isDaySelected(dayStr) { return tempDaysList.indexOf(dayStr) !== -1; }
            function getDaysString() { return tempDaysList.join(", "); }

            ColumnLayout {
                anchors.centerIn: parent; width: 600; spacing: 30
                Text { text: "Planification"; font.pixelSize: 24; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }
                Text { text: "Borne : " + window.selectedStation; color: "#2979FF"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                Rectangle { height: 1; Layout.fillWidth: true; color: "#EEE" }
                Text { text: "Jours :"; color: "#666"; font.bold: true }
                RowLayout {
                    Layout.alignment: Qt.AlignHCenter; spacing: 10
                    Repeater {
                        model: ["Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"]
                        delegate: Rectangle {
                            property bool selected: schedulePageItem.isDaySelected(modelData)
                            width: 70; height: 70; radius: 35
                            color: selected ? "#2979FF" : "#F5F5F5"
                            border.color: selected ? "#2979FF" : "#DDD"; border.width: 1
                            Behavior on color { ColorAnimation { duration: 100 } }
                            Text { anchors.centerIn: parent; text: modelData.substring(0, 3); color: parent.selected ? "white" : "#666"; font.bold: true }
                            MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor; onClicked: schedulePageItem.toggleDay(modelData) }
                        }
                    }
                }
                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 20; spacing: 20
                    ColumnLayout { Layout.fillWidth: true; Text { text: "Début :"; color: "#666" } ComboBox { id: startTimeCombo; Layout.fillWidth: true; model: ["06:00", "07:00", "08:00", "09:00", "12:00"] } }
                    ColumnLayout { Layout.fillWidth: true; Text { text: "Fin :"; color: "#666" } ComboBox { id: endTimeCombo; Layout.fillWidth: true; model: ["14:00", "16:00", "18:00", "20:00"] } }
                }
                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 40
                    AppButton { text: "Retour"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Suivant"; Layout.fillWidth: true
                        enabled: schedulePageItem.tempDaysList.length > 0; opacity: enabled ? 1.0 : 0.5
                        onClicked: window.saveSchedule(schedulePageItem.getDaysString(), startTimeCombo.currentText, endTimeCombo.currentText)
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 4 : LISTE VÉHICULES
    // ============================================================
    Component {
        id: vehiclesPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 40; spacing: 20
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton { text: "←"; onClicked: stackView.pop(); font.pixelSize: 18 }
                    ColumnLayout {
                        Layout.leftMargin: 10
                        Text { text: window.isBookingFlow ? "Choisir le véhicule" : "Mes Véhicules"; font.pixelSize: 28; font.bold: true; color: "#333" }
                        Text { visible: window.isBookingFlow; text: "Planifié : " + window.selectedDays + " | " + window.selectedStartTime + " - " + window.selectedEndTime; color: "#2979FF"; font.bold: true }
                    }
                    Item { Layout.fillWidth: true }
                    AppButton { text: "+ Ajouter"; onClicked: stackView.push(addVehicleFormPage) }
                }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true; spacing: 15
                    model: vehiclesModel
                    delegate: Rectangle {
                        width: parent.width; height: 120; radius: 12; color: "white"
                        border.color: mouseAreaVeh.containsMouse ? "#2979FF" : "#EEF0F2"
                        border.width: mouseAreaVeh.containsMouse ? 2 : 1
                        MouseArea {
                            id: mouseAreaVeh; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                            onClicked: window.selectVehicle(name)
                        }
                        RowLayout {
                            anchors.fill: parent; anchors.margins: 20; spacing: 20
                            Rectangle { width: 80; height: 60; color: "#F5F5F5"; radius: 8; Text { anchors.centerIn: parent; text: "🚗"; font.pixelSize: 30 } }
                            ColumnLayout {
                                Layout.fillWidth: true
                                Text { text: name; font.bold: true; font.pixelSize: 18; color: "#333" }
                                Text { text: "Kilométrage: " + km + " km"; color: "#888" }
                            }
                            RoundButton { text: "🗑️"; font.pixelSize: 16; flat: true; onClicked: window.removeVehicle(index) }
                            Rectangle {
                                height: 36; width: 120; radius: 18; color: "#2979FF"
                                Text { anchors.centerIn: parent; text: window.isBookingFlow ? "Choisir" : "Sélectionner"; color: "white"; font.bold: true }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: addVehicleFormPage
        Item {
            Rectangle { anchors.fill: parent; color: "white" }
            ColumnLayout {
                anchors.centerIn: parent; width: 400; spacing: 20
                Text { text: "Nouveau Véhicule"; font.pixelSize: 24; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }
                AppTextField { id: vName; placeholderText: "Modèle (ex: Peugeot e-208)" }
                AppTextField { id: vYear; placeholderText: "Année (ex: 2024)" }
                AppTextField { id: vKm; placeholderText: "Kilométrage (ex: 12000)" }
                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 20
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Enregistrer"; Layout.fillWidth: true
                        onClicked: {
                            var added = window.addVehicleToModel(vName.text, vYear.text, vKm.text);
                            if (added) { vName.text = ""; vYear.text = ""; vKm.text = ""; stackView.pop(); }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: mileagePage
        Item {
            Rectangle { anchors.fill: parent; color: "white" }
            ColumnLayout {
                anchors.centerIn: parent; width: 500; spacing: 30
                Text { text: "Validation"; font.pixelSize: 24; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }
                Rectangle {
                    Layout.fillWidth: true; height: 140; radius: 8; color: "#E3F2FD"
                    ColumnLayout {
                        anchors.centerIn: parent
                        Text { text: "Borne : " + window.selectedStation; font.bold: true; font.pixelSize: 16 }
                        Text { text: "Véhicule : " + window.selectedVehicle; font.pixelSize: 16 }
                        Rectangle { height: 1; width: 200; color: "#BBDEFB" }
                        Text { text: "Jours : " + window.selectedDays; color: "#1565C0" }
                        Text { text: "Horaire : " + window.selectedStartTime + " à " + window.selectedEndTime; color: "#1565C0" }
                    }
                }
                Text { text: "Confirmer Kilométrage (km) :"; color: "#666" }
                AppTextField { id: kmInput; placeholderText: "Ex: 45200"; inputMethodHints: Qt.ImhDigitsOnly }
                RowLayout {
                    Layout.fillWidth: true; Layout.topMargin: 20
                    AppButton { text: "Retour"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Valider"; Layout.fillWidth: true
                        enabled: kmInput.text !== ""; onClicked: { window.finalizeBooking(kmInput.text); kmInput.text = ""; }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE STATUS & CONTROLE (AVEC DONNÉES GLOBALES)
    // ============================================================
    Component {
        id: consumptionPage
        Item {
            id: consumptionContent
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }

            function formatTime(totalSeconds) {
                var m = Math.floor(totalSeconds / 60);
                var s = totalSeconds % 60;
                return (m < 10 ? "0" + m : m) + ":" + (s < 10 ? "0" + s : s);
            }

            ScrollView {
                anchors.fill: parent; contentWidth: parent.width
                ColumnLayout {
                    width: parent.width; anchors.margins: 40; spacing: 24

                    RowLayout {
                        Layout.fillWidth: true; Layout.topMargin: 40; Layout.leftMargin: 40
                        RoundButton { text: "←"; onClicked: stackView.pop(); font.pixelSize: 18 }
                        Text { text: "Suivi & Contrôle"; font.pixelSize: 28; font.bold: true; color: "#333"; Layout.leftMargin: 10 }
                    }

                    Text { visible: !window.hasActiveSession; text: "Aucune session active."; color: "#666"; font.italic: true; Layout.leftMargin: 40 }

                    Rectangle {
                        id: statusRect
                        visible: window.hasActiveSession
                        Layout.fillWidth: true; Layout.margins: 40; height: 160; radius: 16; color: "white"
                        border.color: "#FF9100"; border.width: 1

                        property string stStatus: window.getSelectedStationStatus()
                        property bool isCharging: (stStatus === "En Charge")

                        RowLayout {
                            anchors.fill: parent; anchors.margins: 24
                            Rectangle { width: 60; height: 60; radius: 30; color: "#FFF3E0"; Text { anchors.centerIn: parent; text: "⚠️"; font.pixelSize: 28 } }
                            ColumnLayout {
                                Text { text: "Commande Manuelle"; font.bold: true; font.pixelSize: 18; color: "#333" }
                                Text { text: "Borne concernée : " + window.selectedStation; font.bold: true; color: "#2979FF" }
                                Text { text: "Véhicule : " + window.selectedVehicle; color: "#673AB7"; font.bold: true }
                                Text { text: "État actuel : " + parent.parent.stStatus; color: "#555" }
                            }
                            Item { Layout.fillWidth: true }
                            AppButton {
                                text: parent.parent.isCharging ? "FORCER L'ARRÊT" : "MARCHE FORCÉE"
                                background: Rectangle { color: parent.parent.parent.isCharging ? "#D32F2F" : "#00C853"; radius: 8 }
                                textColor: "white"
                                onClicked: { window.toggleForcedMode(); }
                            }
                        }
                    }

                    Rectangle {
                        visible: window.hasActiveSession
                        Layout.fillWidth: true; Layout.margins: 40; height: 250; radius: 16; color: "white"
                        ColumnLayout {
                            anchors.centerIn: parent; spacing: 20
                            Text { text: "Session en Temps Réel"; font.bold: true; font.pixelSize: 18; color: "#333"; Layout.alignment: Qt.AlignHCenter }

                            // CHRONO QUI UTILISE LA VARIABLE GLOBALE
                            Text {
                                text: consumptionContent.formatTime(window.sessionDurationSeconds)
                                font.pixelSize: 48; font.bold: true; color: statusRect.isCharging ? "#00C853" : "#CCC"
                                Layout.alignment: Qt.AlignHCenter
                            }

                            GridLayout {
                                columns: 3; columnSpacing: 40
                                ColumnLayout {
                                    Text { text: "Coût Total"; color: "#888"; font.pixelSize: 14; Layout.alignment: Qt.AlignHCenter }
                                    Text {
                                        text: window.sessionCostEuro.toFixed(4) + " €"
                                        font.bold: true; font.pixelSize: 20; color: "#333"; Layout.alignment: Qt.AlignHCenter
                                    }
                                }
                                Rectangle { width: 1; height: 40; color: "#EEE" }
                                ColumnLayout {
                                    Text { text: "Énergie"; color: "#888"; font.pixelSize: 14; Layout.alignment: Qt.AlignHCenter }
                                    Text {
                                        text: window.sessionEnergyKwh.toFixed(3) + " kWh"
                                        font.bold: true; font.pixelSize: 20; color: "#333"; Layout.alignment: Qt.AlignHCenter
                                    }
                                }
                            }

                            Text {
                                text: "Puissance de charge : " + (statusRect.isCharging ? window.userMaxPower : 0) + " kW"
                                color: "#2979FF"; font.bold: true; Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: settingsPage
        Item {
            Rectangle { anchors.fill: parent; color: "white" }
            ColumnLayout {
                anchors.centerIn: parent; width: 400; spacing: 25
                Text { text: "Paramètres Généraux"; font.pixelSize: 24; font.bold: true; color: "#333"; Layout.alignment: Qt.AlignHCenter }
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 5
                    Text { text: "Tarif électricité (€/kWh)"; font.bold: true; color: "#555" }
                    AppTextField { id: costInput; text: window.userCostPerKwh.toString(); placeholderText: "0.20"; inputMethodHints: Qt.ImhFormattedNumbersOnly }
                    Text { text: "Utilisé pour l'estimation des coûts."; color: "#888"; font.pixelSize: 12 }
                }
                ColumnLayout {
                    Layout.fillWidth: true; spacing: 5
                    Text { text: "Puissance Souscrite Max (kW)"; font.bold: true; color: "#555" }
                    AppTextField { id: powerInput; text: window.userMaxPower.toString(); placeholderText: "12"; inputMethodHints: Qt.ImhDigitsOnly }
                    Text { text: "Seuil d'alerte de dépassement."; color: "#888"; font.pixelSize: 12 }
                }
                Item { height: 20 }
                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Sauvegarder"; Layout.fillWidth: true
                        onClicked: {
                            var newCost = parseFloat(costInput.text.replace(",", "."));
                            var newPower = parseInt(powerInput.text);
                            if (!isNaN(newCost)) window.userCostPerKwh = newCost;
                            if (!isNaN(newPower)) window.userMaxPower = newPower;
                            stackView.pop();
                        }
                    }
                }
            }
        }
    }

    component AppButton : Button {
        property bool isPrimary: true
        property color textColor: isPrimary ? "white" : "#333"
        flat: true
        contentItem: Text { text: parent.text; color: parent.pressed ? Qt.darker(textColor, 1.2) : textColor; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter; font.bold: true }
        background: Rectangle { color: isPrimary ? (parent.pressed ? Qt.darker("#2979FF", 1.1) : "#2979FF") : (parent.pressed ? "#EEE" : "white"); radius: 8; border.color: isPrimary ? "transparent" : "#DDD"; border.width: isPrimary ? 0 : 1 }
    }
    component AppTextField : TextField {
        Layout.fillWidth: true
        background: Rectangle { color: "#F5F5F5"; radius: 8; border.color: parent.activeFocus ? "#2979FF" : "transparent"; border.width: 2 }
        color: "#333"; placeholderTextColor: "#AAA"
    }
    component AppCard : Rectangle {
        id: cardRoot
        property string iconChar; property color iconBg; property string title; property string subTitle; property string actionText
        signal clicked()
        Layout.fillWidth: true; height: 200; radius: 16; color: "white"
        border.color: mouseArea.containsMouse ? "#2979FF" : "transparent"; border.width: mouseArea.containsMouse ? 2 : 0
        scale: mouseArea.pressed ? 0.98 : (mouseArea.containsMouse ? 1.02 : 1.0)
        Behavior on scale { NumberAnimation { duration: 100 } }
        MouseArea { id: mouseArea; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor; onClicked: cardRoot.clicked() }
        ColumnLayout {
            anchors.fill: parent; anchors.margins: 24; spacing: 12
            Rectangle { width: 48; height: 48; radius: 12; color: iconBg; Text { anchors.centerIn: parent; text: iconChar; color: "white"; font.pixelSize: 24 } }
            Text { text: title; font.bold: true; font.pixelSize: 18; color: "#333"; Layout.topMargin: 10 }
            Text { text: subTitle; font.pixelSize: 14; color: "#888"; Layout.fillWidth: true; wrapMode: Text.WordWrap }
            Text { text: actionText; color: "#00C853"; font.bold: true; font.pixelSize: 14 }
        }
    }
}
