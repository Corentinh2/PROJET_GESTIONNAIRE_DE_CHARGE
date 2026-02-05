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

    property string selectedStation: ""
    property string selectedVehicle: ""

    property bool isBookingFlow: false

    property string selectedDays: ""
    property string selectedStartTime: ""
    property string selectedEndTime: ""
    property string selectedMileage: ""
    property bool hasActiveSession: false

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

    // Modèle d'Affichage Bornes
    ListModel {
        id: stationsModelDisplay
        Component.onCompleted: window.updateStationFilter(false)
    }

    property bool showOnlyAvailable: false

    // --- LOGIQUE METIER ---

    // AJOUTER BORNE
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

    // SUPPRIMER BORNE (NOUVEAU)
    function removeStation(stationName) {
        var removed = false;
        // On parcourt à l'envers pour supprimer sans casser les index
        for (var i = stationsModelSource.count - 1; i >= 0; i--) {
            var item = stationsModelSource.get(i);
            if (item.name === stationName) {
                // Si on supprime la borne active, on reset la session
                if (window.selectedStation === stationName) {
                    window.resetSession();
                }
                stationsModelSource.remove(i);
                removed = true;
            }
        }
        window.updateStationFilter(window.showOnlyAvailable);
        return removed;
    }

    // AJOUTER VÉHICULE
    function addVehicleToModel(name, year, km) {
        var success = false;
        if (name !== "") {
            vehiclesModel.append({
                "name": name, "year": year, "batt": "N/A", "km": km
            });
            success = true;
        }
        return success;
    }

    // SUPPRIMER VÉHICULE (NOUVEAU)
    function removeVehicle(index) {
        var item = vehiclesModel.get(index);
        // Si c'est le véhicule sélectionné, on le désélectionne
        if (item.name === window.selectedVehicle) {
            window.selectedVehicle = "";
        }
        vehiclesModel.remove(index);
        return true;
    }

    // 1. Démarrer le processus de réservation
    function startBookingProcess(stationName) {
        window.selectedStation = stationName;
        window.isBookingFlow = true;

        window.selectedDays = "";
        window.selectedStartTime = "";
        window.selectedEndTime = "";

        stackView.push(schedulePage);
        return true;
    }

    // 2. Enregistrer l'horaire
    function saveSchedule(daysStr, startStr, endStr) {
        window.selectedDays = daysStr;
        window.selectedStartTime = startStr;
        window.selectedEndTime = endStr;

        if (window.selectedVehicle !== "") {
            stackView.push(mileagePage);
        } else {
            stackView.push(vehiclesPage);
        }
        return true;
    }

    // 3. Sélectionner le véhicule
    function selectVehicle(vehicleName) {
        window.selectedVehicle = vehicleName;

        if (window.isBookingFlow) {
            stackView.push(mileagePage);
        } else {
            stackView.pop(null);
        }
        return true;
    }

    // 4. Finaliser
    function finalizeBooking(km) {
        window.selectedMileage = km;
        window.hasActiveSession = true;

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

    // Reset
    function resetSession() {
        if (window.selectedStation !== "") {
            for (var i = 0; i < stationsModelSource.count; i++) {
                var item = stationsModelSource.get(i);
                if (item.name === window.selectedStation) {
                    stationsModelSource.setProperty(i, "status", "Disponible");
                }
            }
        }
        window.selectedStation = "";
        window.selectedDays = "";
        window.selectedStartTime = "";
        window.selectedEndTime = "";
        window.selectedMileage = "";
        window.hasActiveSession = false;
        window.isBookingFlow = false;

        window.updateStationFilter(window.showOnlyAvailable);
        return true;
    }

    // Marche Forcée
    function toggleForcedMode() {
        var currentState = "Inconnu";
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
                if (item.status !== "Disponible") {
                    shouldAdd = false;
                }
            }
            if (shouldAdd) {
                stationsModelDisplay.append(item);
            }
        }
        return true;
    }

    // --- UTILS GRAPHIQUES ---
    function getStatusColor(status) {
        var col = "#888";
        if (status === "Disponible") { col = "#00C853"; }
        else {
            if (status === "Occupée") { col = "#FF5252"; }
            else {
                if (status === "Programmé") { col = "#2979FF"; }
                else {
                     if (status === "En Charge") { col = "#FF9100"; }
                }
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
                else {
                    if (status === "En Charge") { col = "#FFF3E0"; }
                }
            }
        }
        return col;
    }

    // --- NAVIGATION ---
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
    // PAGE 1 : DASHBOARD
    // ============================================================
    Component {
        id: dashboardPage
        Item {
            ScrollView {
                anchors.fill: parent; contentWidth: parent.width
                ColumnLayout {
                    width: parent.width; spacing: 24; anchors.margins: 40

                    // Header
                    RowLayout {
                        Layout.fillWidth: true; Layout.margins: 40; Layout.topMargin: 40
                        Rectangle { width: 48; height: 48; radius: 12; color: "#00C853"; Text { anchors.centerIn: parent; text: "⚡"; color: "white"; font.pixelSize: 24 } }
                        ColumnLayout {
                            Text { text: "EV Charge Manager"; font.bold: true; font.pixelSize: 18; color: "#333" }
                            Text { text: "Gérez vos recharges facilement"; font.pixelSize: 14; color: "#888" }
                        }
                        Item { Layout.fillWidth: true }

                        Rectangle {
                            visible: window.selectedVehicle !== ""
                            width: 200; height: 40; radius: 20; color: "#EDE7F6"
                            RowLayout {
                                anchors.centerIn: parent
                                Text { text: "🚗"; font.pixelSize: 16 }
                                Text { text: window.selectedVehicle; font.bold: true; color: "#673AB7" }
                            }
                        }
                        AppButton { text: "Déconnexion"; isPrimary: false; textColor: "#FF5252"; onClicked: print("Logout") }
                    }

                    // --- BANDEAU D'ETAT ---
                    Rectangle {
                        visible: window.hasActiveSession
                        Layout.fillWidth: true; Layout.leftMargin: 40; Layout.rightMargin: 40
                        height: 140; radius: 12; color: "#E3F2FD"
                        border.color: "#2979FF"; border.width: 1

                        RowLayout {
                            anchors.fill: parent; anchors.margins: 20
                            Rectangle { width: 50; height: 50; radius: 25; color: "#2979FF"; Text { anchors.centerIn: parent; text: "ℹ️"; font.pixelSize: 24 } }

                            ColumnLayout {
                                Layout.fillWidth: true
                                Text { text: "Session Programmée en cours"; font.bold: true; color: "#1565C0" }
                                Text {
                                    text: "Borne: " + window.selectedStation + "\n" +
                                          "Véhicule: " + window.selectedVehicle + "\n" +
                                          "Jours: " + window.selectedDays + "\n" +
                                          "Horaire: " + window.selectedStartTime + " à " + window.selectedEndTime
                                    color: "#555"
                                }
                            }
                            AppButton { text: "Réinitialiser"; isPrimary: false; textColor: "#D32F2F"; onClicked: window.resetSession() }
                        }
                    }

                    ColumnLayout { Layout.leftMargin: 40
                        Text { text: "Bienvenue, Utilisateur !"; font.pixelSize: 32; font.bold: true; color: "#1a1a1a" }
                    }

                    GridLayout {
                        columns: 3; Layout.fillWidth: true; Layout.leftMargin: 40; Layout.rightMargin: 40; columnSpacing: 20; rowSpacing: 20

                        AppCard {
                            iconChar: "🔌"; iconBg: "#2979FF"; title: "Bornes"; subTitle: "Programmer une charge"; actionText: "Démarrer >"
                            onClicked: {
                                if (window.hasActiveSession) { print("Déjà une session active"); }
                                stackView.push(stationsPage);
                            }
                        }
                        AppCard {
                            iconChar: "🚗"; iconBg: "#D500F9"; title: "Mes Véhicules"; subTitle: "Gérer la flotte"; actionText: "Accéder >"
                            onClicked: {
                                window.isBookingFlow = false;
                                stackView.push(vehiclesPage);
                            }
                        }
                        AppCard {
                            iconChar: "📊"; iconBg: "#00C853"; title: "Contrôle & Status"; subTitle: "Marche forcée et stats"; actionText: "Accéder >"
                            onClicked: stackView.push(consumptionPage)
                        }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 2 : LISTE BORNES (+ SUPPRESSION)
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
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: (status === "Disponible") ? Qt.PointingHandCursor : Qt.ArrowCursor
                            onClicked: {
                                if (status === "Disponible") { window.startBookingProcess(name); }
                            }
                        }

                        Rectangle {
                            width: 6; height: parent.height; color: window.getStatusColor(status)
                            radius: 12; Rectangle { width: 3; height: parent.height; color: parent.color; anchors.right: parent.right }
                        }

                        Item {
                            anchors.fill: parent; anchors.margins: 15; anchors.leftMargin: 20
                            RowLayout {
                                anchors.top: parent.top; width: parent.width
                                Rectangle {
                                    width: 48; height: 48; radius: 12; color: window.getStatusBgColor(status)
                                    Text { anchors.centerIn: parent; text: "⚡"; font.pixelSize: 24; color: window.getStatusColor(status) }
                                }
                                ColumnLayout {
                                    Text { text: name; font.bold: true; font.pixelSize: 16; color: "#333" }
                                    Text { text: "📍 " + loc + " • " + kwh; color: "#888"; font.pixelSize: 13 }
                                }
                                Item { Layout.fillWidth: true }
                                // BOUTON SUPPRIMER BORNE
                                RoundButton {
                                    text: "🗑️"
                                    font.pixelSize: 16
                                    flat: true
                                    onClicked: window.removeStation(name)
                                }
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

    // ============================================================
    // PAGE 2 BIS : FORMULAIRE AJOUT BORNE
    // ============================================================
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
    // PAGE 4 : LISTE VÉHICULES (+ AJOUT ET SUPPRESSION)
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
                        Text {
                            visible: window.isBookingFlow
                            text: "Planifié : " + window.selectedDays + " | " + window.selectedStartTime + " - " + window.selectedEndTime
                            color: "#2979FF"; font.bold: true
                        }
                    }
                    Item { Layout.fillWidth: true }
                    // BOUTON AJOUTER VÉHICULE
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
                            id: mouseAreaVeh
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
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
                            // BOUTON SUPPRIMER VÉHICULE
                            RoundButton {
                                text: "🗑️"
                                font.pixelSize: 16
                                flat: true
                                onClicked: window.removeVehicle(index)
                            }
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

    // ============================================================
    // PAGE 4 BIS : FORMULAIRE VÉHICULE (AJOUT)
    // ============================================================
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

    // ============================================================
    // PAGE 5 : SAISIE KILOMETRAGE
    // ============================================================
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
    // PAGE STATUS & CONTROLE
    // ============================================================
    Component {
        id: consumptionPage
        Item {
            Rectangle { anchors.fill: parent; color: "#F4F6F9" }
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
                }
            }
        }
    }

    // ============================================================
    // COMPOSANTS UI
    // ============================================================
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
