// Importation des modules de base QtQuick pour l'interface graphique
import QtQuick 2.15
// Importation des modules pour la gestion des mises en page (RowLayout, ColumnLayout)
import QtQuick.Layouts 1.15
// Importation des composants d'interface standards (Boutons, Champs de texte, StackView)
import QtQuick.Controls 2.15

/**
 * @brief Fenêtre principale de l'application.
 * Elle définit les dimensions et les propriétés globales de la fenêtre système.
 */
ApplicationWindow {
    id: window

    // --- FORMAT MOBILE ---
    width: 360  // Largeur typique d'un smartphone en pixels
    height: 720 // Hauteur typique d'un smartphone en pixels
    visible: true // Rend la fenêtre visible au lancement
    title: "Gestionnaire de charge" // Titre de la fenêtre système

    // Le fond de la fenêtre représente l'extérieur du téléphone (la table ou l'arrière-plan)
    color: "#1E1E1E"
    font.family: "Segoe UI" // Police de caractères globale de l'application

    // --- DONNÉES GLOBALES ET ÉTAT ---
    // Ces propriétés agissent comme des variables globales accessibles de partout dans l'application.

    property string activeStation: ""      // Nom de la borne actuellement contrôlée
    property string selectedVehicle: ""    // Véhicule sélectionné pour la charge
    property bool isBookingFlow: false     // Indique si l'utilisateur est en train de programmer une session
    property string selectedDays: ""       // Jours choisis pour la programmation
    property string selectedStartTime: ""  // Heure de début programmée
    property string selectedEndTime: ""    // Heure de fin programmée
    property string selectedMileage: ""    // Kilométrage saisi lors de la validation

    // --- PARAMÈTRES UTILISATEUR ---
    property double userCostPerKwh: 0.20   // Tarif de l'électricité actuel (modifiable dans les paramètres)
    property int userMaxPower: 12          // Seuil de puissance maximale avant alerte

    // --- VARIABLES DE SESSION EN TEMPS RÉEL ---
    property int sessionDurationSeconds: 0 // Compteur de temps de la charge en cours
    property double sessionEnergyKwh: 0.0  // Énergie consommée calculée
    property double sessionCostEuro: 0.0   // Coût calculé en fonction de l'énergie et du tarif

    /**
     * @brief Modèle de données stockant les notifications (alertes).
     * Alimenté dynamiquement par le Timer de simulation ou le serveur.
     */
    ListModel {
        id: notificationModel
    }

    /**
     * @brief Boucle principale de simulation (Timer).
     * S'exécute toutes les secondes uniquement si une borne est "En Charge".
     */
    Timer {
        id: globalChargingTimer
        interval: 1000 // Se déclenche toutes les 1000 ms (1 seconde)
        repeat: true   // Tourne en boucle
        // Condition de fonctionnement : une borne est sélectionnée ET son statut est "En Charge"
        running: (window.activeStation !== "") && (window.getSelectedStationStatus() === "En Charge")

        onTriggered: {
            // Incrémentation du temps
            window.sessionDurationSeconds += 1;

            // Simulation de la puissance consommée (kW)
            var powerKw = window.userMaxPower;
            // Conversion de la puissance (kW) en énergie (kWh) pour 1 seconde
            var energyAdded = powerKw / 3600.0;

            // Mise à jour des totaux
            window.sessionEnergyKwh += energyAdded;
            window.sessionCostEuro = window.sessionEnergyKwh * window.userCostPerKwh;

            // Détection de surtension (sécurité)
            if (powerKw > 20) {
                if (notificationModel.count === 0) { // On n'ajoute l'alerte que si la liste est vide
                    notificationModel.append({
                        "type": "SURTENSION DÉTECTÉE",
                        "message": "Puissance critique dépassée (" + powerKw + " kW) sur " + window.activeStation
                    });
                }
            }

            // Simulation aléatoire d'une surchauffe (1 chance sur 100 chaque seconde)
            var rand = Math.random();
            if (rand > 0.99) {
                if (notificationModel.count === 0) {
                    if (powerKw <= 20) {
                        notificationModel.append({
                            "type": "ALERTE SURCHAUFFE",
                            "message": "Température anormale du câble sur " + window.activeStation
                        });
                    }
                }
            }
        }
    }



    // --- BASES DE DONNÉES LOCALES (MODÈLES) ---
    // Ces modèles servent de source de données pour les listes graphiques (ListView).

    ListModel { id: sessionsModel } // Modèle vide au démarrage, rempli par l'utilisateur

    ListModel {
        id: vehiclesModel
        // Données factices pour l'affichage initial
        ListElement { name: "Tesla Model 3"; year: "2023"; km: "12500" }
        ListElement { name: "Renault Zoe"; year: "2022"; km: "42300" }
        ListElement { name: "Volkswagen ID.4"; year: "2024"; km: "5600" }
    }

    ListModel {
        id: stationsModelSource
        // Données factices pour la sélection initiale
        ListElement { name: "Borne A"; kwh: "22 kW"; status: "Disponible" }
        ListElement { name: "Borne B"; kwh: "22 kW"; status: "Disponible" }
        ListElement { name: "Borne C"; kwh: "50 kW"; status: "Disponible" }
    }

    // ============================================================
    // LOGIQUE METIER STRICTE (JAVASCRIPT)
    // ============================================================

    /**
     * @brief Récupère le kilométrage d'un véhicule spécifique.
     * @param vName Le nom du véhicule à chercher.
     * @return int Le kilométrage actuel du véhicule (0 si non trouvé).
     */
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

    /**
     * @brief Définit la borne sur laquelle l'application est connectée.
     * @param stationName Le nom de la borne sélectionnée.
     * @return bool Toujours true, confirme l'exécution de l'action.
     */
    function selectActiveStation(stationName) {
        var success = true;
        window.activeStation = stationName;
        // Remplace la page d'accueil (bootPage) par le tableau de bord (dashboardPage)
        stackView.replace(null, dashboardPage);
        return success;
    }

    /**
     * @brief Ajoute un nouveau véhicule dans le modèle de données.
     * @param name Le nom/modèle du véhicule.
     * @param year L'année du véhicule.
     * @param km Le kilométrage initial.
     * @return bool true si le nom n'est pas vide et l'ajout effectué, false sinon.
     */
    function addVehicleToModel(name, year, km) {
        var success = false;
        if (name !== "") {
            vehiclesModel.append({ "name": name, "year": year, "km": km });
            success = true;
        }
        return success;
    }

    /**
     * @brief Supprime un véhicule du modèle en fonction de son index.
     * @param index La position du véhicule dans la liste.
     * @return bool Toujours true.
     */
    function removeVehicle(index) {
        var success = true;
        var item = vehiclesModel.get(index);
        // Si le véhicule supprimé était celui sélectionné, on efface la sélection
        if (item.name === window.selectedVehicle) {
            window.selectedVehicle = "";
        }
        vehiclesModel.remove(index);
        return success;
    }

    /**
     * @brief Initialise le flux de création d'une nouvelle session de charge.
     * @return bool Toujours true.
     */
    function startBookingProcess() {
        var success = true;
        window.isBookingFlow = true;
        window.selectedDays = "";
        window.selectedStartTime = "";
        window.selectedEndTime = "";
        // Ouvre la page du calendrier
        stackView.push(schedulePage);
        return success;
    }

    /**
     * @brief Sauvegarde les horaires choisis et oriente vers la prochaine étape.
     * @param daysStr Chaine de caractères contenant les jours sélectionnés.
     * @param startStr Heure de début.
     * @param endStr Heure de fin.
     * @return bool Toujours true.
     */
    function saveSchedule(daysStr, startStr, endStr) {
        var success = true;
        window.selectedDays = daysStr;
        window.selectedStartTime = startStr;

        // Extraction des heures pour vérifier si la fin est le lendemain
        var startHour = parseInt(startStr.substring(0, 2));
        var endHour = parseInt(endStr.substring(0, 2));
        var finalEndStr = endStr;

        // Logique heure creuse (passage à minuit)
        if (endHour < startHour) {
            finalEndStr = endStr + " (lendemain)";
        }
        window.selectedEndTime = finalEndStr;

        var hasVehicle = false;
        if (window.selectedVehicle !== "") {
            hasVehicle = true;
        }

        // Aiguillage : si on a déjà un véhicule, on passe au kilométrage, sinon on choisit le véhicule
        if (hasVehicle) {
            stackView.push(mileagePage);
        }
        if (!hasVehicle) {
            stackView.push(vehiclesPage);
        }
        return success;
    }

    /**
     * @brief Enregistre le véhicule sélectionné par l'utilisateur.
     * @param vehicleName Le nom du véhicule choisi.
     * @return bool Toujours true.
     */
    function selectVehicle(vehicleName) {
        var success = true;
        window.selectedVehicle = vehicleName;
        // Comportement différent si on est en train de programmer une charge ou juste en consultation
        if (window.isBookingFlow) {
            stackView.push(mileagePage);
        }
        if (!window.isBookingFlow) {
            stackView.pop(null); // Retour à la page précédente
        }
        return success;
    }

    /**
     * @brief Finalise et enregistre la session de charge programmée.
     * @param km Le kilométrage saisi par l'utilisateur lors de la validation.
     * @return bool true si la borne associée a été trouvée et mise à jour.
     */
    function finalizeBooking(km) {
        var found = false;
        window.selectedMileage = km;

        // Met à jour le kilométrage du véhicule dans la base
        for (var v = 0; v < vehiclesModel.count; v++) {
            var vItem = vehiclesModel.get(v);
            if (vItem.name === window.selectedVehicle) {
                vehiclesModel.setProperty(v, "km", km);
            }
        }

        // Ajoute la session dans la liste des charges prévues
        sessionsModel.append({
            "station": window.activeStation,
            "vehicle": window.selectedVehicle,
            "days": window.selectedDays,
            "start": window.selectedStartTime,
            "end": window.selectedEndTime
        });

        // Bascule le statut de la borne vers "Programmé" si elle était "Disponible"
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
        // Retourne tout au début de la pile d'écrans (retour au Dashboard)
        stackView.pop(null);
        return found;
    }

    /**
     * @brief Annule et supprime une session programmée.
     * @param index La position de la session dans la liste.
     * @return bool true si la session a été supprimée.
     */
    function deleteSession(index) {
        var success = false;
        if (index >= 0) {
            if (index < sessionsModel.count) {
                sessionsModel.remove(index);
                success = true;
            }
        }

        // Compte s'il reste d'autres sessions actives sur cette même borne
        var remainingSessions = 0;
        for (var k = 0; k < sessionsModel.count; k++) {
            var s = sessionsModel.get(k);
            if (s.station === window.activeStation) {
                remainingSessions = remainingSessions + 1;
            }
        }

        // S'il n'y a plus de session, la borne repasse en état "Disponible" (sauf si elle charge actuellement)
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

    /**
     * @brief Bascule l'état manuel de la borne (Force la charge ou l'arrête).
     * @return string Le nouvel état opérationnel ("Marche", "Arrêt" ou "Inconnu").
     */
    function toggleForcedMode() {
        var currentState = "Inconnu";
        var actionDone = false;

        for (var i = 0; i < stationsModelSource.count; i++) {
            var item = stationsModelSource.get(i);
            if (item.name === window.activeStation) {

                // Si la borne est dispo, on force la charge
                if (!actionDone) {
                    if (item.status === "Disponible") {
                        stationsModelSource.setProperty(i, "status", "En Charge");
                        currentState = "Marche";
                        actionDone = true;
                    }
                }
                // Si la borne est programmée, on force la charge immédiatement
                if (!actionDone) {
                    if (item.status === "Programmé") {
                        stationsModelSource.setProperty(i, "status", "En Charge");
                        currentState = "Marche";
                        actionDone = true;
                    }
                }
                // Si la borne est en charge, on l'arrête
                if (!actionDone) {
                    if (item.status === "En Charge") {
                        var hasSessions = false;
                        for (var k = 0; k < sessionsModel.count; k++) {
                            var s = sessionsModel.get(k);
                            if (s.station === window.activeStation) {
                                hasSessions = true;
                            }
                        }
                        // Restaure l'état précédent (Programmé ou Disponible)
                        if (hasSessions) {
                            stationsModelSource.setProperty(i, "status", "Programmé");
                        }
                        if (!hasSessions) {
                            stationsModelSource.setProperty(i, "status", "Disponible");
                            // Remise à zéro des compteurs de session
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

    /**
     * @brief Lit l'état de la borne sélectionnée.
     * @return string L'état actuel ("Disponible", "En Charge", "Programmé", etc.).
     */
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

    // ============================================================
    // GABARIT DU TELEPHONE (LA COQUE ET LA ZONE D'AFFICHAGE)
    // ============================================================
    Rectangle {
        id: appFrame
        anchors.fill: parent

        // Espace extérieur (Marge entre la fenêtre du PC et le faux téléphone)
        anchors.margins: 2

        radius: 30               // Arrondi du téléphone
        color: "#F4F6F9"         // Couleur de fond des pages (l'écran)

        // Épaisseur de la coque (La zone noire)
        border.color: "#000000"
        border.width: 5
        clip: true               // Coupe visuellement tout élément qui dépasse des arrondis

        // --- NAVIGATION (Le moteur de changement de pages) ---
        StackView {
            id: stackView
            anchors.fill: parent

            // "Safe Area" : Protège le contenu des bordures noires du téléphone
            anchors.topMargin: 15
            anchors.bottomMargin: 10
            anchors.leftMargin: 8
            anchors.rightMargin: 8

            initialItem: bootPage // La page chargée au démarrage

            // Animations de transition fluides (Glissement et Fondu)
            pushEnter: Transition { PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } NumberAnimation { property: "x"; from: 100; to: 0; duration: 200; easing.type: Easing.OutCubic } }
            pushExit: Transition { PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } }
            popEnter: Transition { PropertyAnimation { property: "opacity"; from: 0; to: 1; duration: 200 } }
            popExit: Transition { PropertyAnimation { property: "opacity"; from: 1; to: 0; duration: 200 } NumberAnimation { property: "x"; from: 0; to: 100; duration: 200; easing.type: Easing.InCubic } }
        }
    }

    // ============================================================
    // PAGE 0 : SÉLECTION DE LA BORNE
    // ============================================================
    Component {
        id: bootPage
        Item {
            // Fond transparent pour hériter de la couleur de l'appFrame
            Rectangle { anchors.fill: parent; color: "transparent" }
            ColumnLayout {
                anchors.fill: parent; spacing: 20
                Item { Layout.fillHeight: true } // Repousse le contenu vers le centre
                Text {
                    text: "Gestionnaire de charge"
                    font.pixelSize: 22; font.bold: true; color: "#2979FF"
                    Layout.alignment: Qt.AlignHCenter
                }
                Text {
                    text: "Sélectionnez votre borne"
                    font.pixelSize: 16; color: "#666"
                    Layout.alignment: Qt.AlignHCenter
                }
                // Liste générée automatiquement à partir de stationsModelSource
                ListView {
                    Layout.fillWidth: true; height: 250; clip: true; spacing: 10
                    model: stationsModelSource
                    delegate: Rectangle { // Design de chaque ligne de la liste
                        width: parent.width; height: 70; radius: 8; color: "white"; border.color: "#DDD"; border.width: 1
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
    // PAGE 1 : DASHBOARD (Menu Principal)
    // ============================================================
    Component {
        id: dashboardPage
        Item {
            Rectangle { anchors.fill: parent; color: "transparent" }
            ScrollView {
                anchors.fill: parent; contentWidth: parent.width
                ColumnLayout {
                    width: parent.width; spacing: 20
                    // En-tête avec nom de la borne et bouton paramètres
                    RowLayout {
                        Layout.fillWidth: true
                        ColumnLayout {
                            Text { text: "Borne : " + window.activeStation; font.bold: true; font.pixelSize: 20; color: "#2979FF" }
                            Text { text: "Supervision"; font.pixelSize: 14; color: "#888" }
                        }
                        Item { Layout.fillWidth: true }
                        RoundButton {
                            text: "⚙️"; font.pixelSize: 18
                            onClicked: stackView.push(settingsPage)
                        }
                    }

                    AppButton {
                        text: "Changer de borne"; isPrimary: false; Layout.fillWidth: true
                        onClicked: stackView.replace(null, bootPage)
                    }

                    // --- ZONE D'ALERTES (Visible uniquement si erreur) ---
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

                    // --- BOUTONS DU MENU PRINCIPAL ---
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
    // PAGE 2 : GESTION DES SESSIONS PROGRAMMÉES
    // ============================================================
    Component {
        id: sessionsPage
        Item {
            Rectangle { anchors.fill: parent; color: "transparent" }
            ColumnLayout {
                anchors.fill: parent; spacing: 15
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
                        width: parent.width; height: 90; radius: 8; color: "white"; border.color: "#DDD"; border.width: 1
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
    // PAGE 3 : CALENDRIER (Sélection des jours et heures)
    // ============================================================
    Component {
        id: schedulePage
        Item {
            id: schedulePageItem
            Rectangle { anchors.fill: parent; color: "transparent" }

            // Liste temporaire pour stocker les jours cochés par l'utilisateur
            property var tempDaysList: []

            /**
             * @brief Ajoute ou retire un jour de la sélection.
             * @param dayStr Le nom du jour cliqué.
             */
            function toggleDay(dayStr) {
                var list = [];
                for (var i = 0; i < tempDaysList.length; i++) { list.push(tempDaysList[i]); }
                var idx = list.indexOf(dayStr);
                if (idx !== -1) { list.splice(idx, 1); } // Retire si existe déjà
                if (idx === -1) { list.push(dayStr); }  // Ajoute si n'existe pas
                tempDaysList = list;
            }

            /**
             * @brief Vérifie si un jour fait partie de la sélection actuelle.
             * @param dayStr Le jour à vérifier.
             * @return bool true si le jour est sélectionné.
             */
            function isDaySelected(dayStr) {
                var result = false;
                if (tempDaysList.indexOf(dayStr) !== -1) { result = true; }
                return result;
            }

            /**
             * @brief Transforme le tableau des jours en une seule chaine de caractères.
             * @return string Les jours séparés par des virgules.
             */
            function getDaysString() {
                var resultStr = tempDaysList.join(", ");
                return resultStr;
            }

            ColumnLayout {
                anchors.fill: parent; spacing: 20
                Text { text: "Jours de charge :"; font.bold: true; font.pixelSize: 16 }

                // Grille pour afficher les pastilles de jours
                GridLayout {
                    Layout.fillWidth: true; columns: 4; rowSpacing: 10; columnSpacing: 10
                    Repeater {
                        model: ["Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"]
                        delegate: Rectangle {
                            property bool selected: schedulePageItem.isDaySelected(modelData)
                            Layout.fillWidth: true; height: 50; radius: 25
                            color: selected ? "#2979FF" : "#DDD" // Change de couleur si coché
                            Text { anchors.centerIn: parent; text: modelData; color: parent.selected ? "white" : "black"; font.bold: parent.selected }
                            MouseArea { anchors.fill: parent; onClicked: schedulePageItem.toggleDay(modelData) }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true; spacing: 10; Layout.topMargin: 10
                    Text { text: "Heure de début :"; color: "#666" }
                    ComboBox { id: sTime; Layout.fillWidth: true; model: [
                            "00:00","01:00","02:00","03:00","04:00","05:00",
                            "06:00","07:00", "08:00", "09:00","10:00","11:00",
                            "12:00","13:00","14:00","15:00","16:00","17:00",
                            "18:00","19:00","20:00","21:00","22:00","23:00"] }
                    Text { text: "Heure de fin :"; color: "#666" }
                    ComboBox { id: eTime; Layout.fillWidth: true; model: [
                            "00:00","01:00","02:00","03:00","04:00","05:00",
                            "06:00","07:00", "08:00", "09:00","10:00","11:00",
                            "12:00","13:00","14:00","15:00","16:00","17:00",
                            "18:00","19:00","20:00","21:00","22:00","23:00"] }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    AppButton { text: "Annuler"; isPrimary: false; Layout.fillWidth: true; onClicked: stackView.pop() }
                    AppButton {
                        text: "Suivant"; Layout.fillWidth: true
                        enabled: schedulePageItem.tempDaysList.length > 0 // Bouton inactif si aucun jour n'est sélectionné
                        onClicked: window.saveSchedule(schedulePageItem.getDaysString(), sTime.currentText, eTime.currentText)
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE 4 : LISTE DES VÉHICULES
    // ============================================================
    Component {
        id: vehiclesPage
        Item {
            Rectangle { anchors.fill: parent; color: "transparent" }
            ColumnLayout {
                anchors.fill: parent; spacing: 15
                RowLayout {
                    Layout.fillWidth: true
                    RoundButton { text: "←"; onClicked: stackView.pop() }
                    Text { text: "Véhicules"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
                }

                AppButton { text: "+ Ajouter Véhicule"; Layout.fillWidth: true; onClicked: stackView.push(addVehicleFormPage) }

                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; spacing: 10; model: vehiclesModel
                    delegate: Rectangle {
                        width: parent.width; height: 70; radius: 8; color: "white"; border.color: "#DDD"; border.width: 1
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

    // ============================================================
    // PAGE D'AJOUT D'UN VÉHICULE (Formulaire)
    // ============================================================
    Component {
        id: addVehicleFormPage
        Item {
            Rectangle { anchors.fill: parent; color: "transparent" }
            ColumnLayout {
                anchors.fill: parent; spacing: 20
                Text { text: "Nouveau Véhicule"; font.pixelSize: 20; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 160; radius: 12; color: "white"; border.color: "#DDD"; border.width: 1
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 15; spacing: 10
                        Text { text: "Modèle :"; font.bold: true }
                        AppTextField { id: vName; placeholderText: "Ex: Renault Zoe" }
                        Text { text: "Kilométrage actuel :"; font.bold: true }
                        // Le clavier affiché sur mobile sera de type numérique
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
    // PAGE 5 : VALIDATION DU KILOMÉTRAGE (Avec Sécurité)
    // ============================================================
    Component {
        id: mileagePage
        Item {
            id: mileageRoot
            Rectangle { anchors.fill: parent; color: "transparent" }

            property int currentVehicleKm: window.getVehicleMileage(window.selectedVehicle)

            ColumnLayout {
                anchors.fill: parent; spacing: 20
                Text { text: "Validation"; font.pixelSize: 20; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"; border.color: "#DDD"; border.width: 1
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
                        // Ce texte rouge apparaît uniquement si l'utilisateur saisit une valeur incohérente
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
                        // Sécurité : le bouton reste grisé tant que la valeur saisie est inférieure à l'ancienne
                        enabled: kmInput.text !== "" && parseInt(kmInput.text) >= mileageRoot.currentVehicleKm
                        onClicked: { window.finalizeBooking(kmInput.text); kmInput.text = ""; }
                    }
                }
            }
        }
    }

    // ============================================================
    // PAGE STATUT & CONTROLE (Tableau de bord temps réel)
    // ============================================================
    Component {
        id: consumptionPage
        Item {
            id: consumptionContent
            Rectangle { anchors.fill: parent; color: "transparent" }

            property string currentVisualState: "Inconnu"

            // Mise à jour continue de l'affichage de l'état
            Timer {
                interval: 500; running: true; repeat: true
                onTriggered: { consumptionContent.currentVisualState = window.getSelectedStationStatus(); }
            }

            /**
             * @brief Convertit les secondes en format "MM:SS".
             * @param totalSeconds Le temps total écoulé en secondes.
             * @return string La durée formatée.
             */
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
                anchors.fill: parent; spacing: 20
                RowLayout {
                    RoundButton { text: "←"; onClicked: stackView.pop() }
                    Text { text: "Statut Borne"; font.pixelSize: 20; font.bold: true; Layout.leftMargin: 10 }
                }

                // Bannière dynamique qui change de couleur selon l'état de la borne
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

                // Affichage du chronomètre et des statistiques, visible uniquement si en charge
                Rectangle {
                    visible: consumptionContent.currentVisualState === "En Charge"
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"; border.color: "#DDD"; border.width: 1
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
    // PAGE DES PARAMÈTRES GLOBAUX
    // ============================================================
    Component {
        id: settingsPage
        Item {
            Rectangle { anchors.fill: parent; color: "transparent" }
            ColumnLayout {
                anchors.fill: parent; spacing: 20
                Text { text: "Paramètres"; font.pixelSize: 24; font.bold: true }

                Rectangle {
                    Layout.fillWidth: true; height: 180; radius: 12; color: "white"; border.color: "#DDD"; border.width: 1
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
                            // Mémorisation uniquement si les valeurs entrées sont des nombres valides
                            if (!isNaN(newCost)) { window.userCostPerKwh = newCost; }
                            if (!isNaN(newPower)) { window.userMaxPower = newPower; }
                            stackView.pop();
                        }
                    }
                }
            }
        }
    }

    // ============================================================
    // COMPOSANTS GRAPHIQUES PERSONNALISÉS (Gabarits réutilisables)
    // ============================================================

    // Bouton standard de l'application
    component AppButton : Button {
        property bool isPrimary: true // Définit s'il s'agit d'un bouton d'action principal (couleur forte)
        property color textColor: isPrimary ? "white" : "black"
        Layout.preferredHeight: 45
        contentItem: Text { text: parent.text; color: textColor; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
        background: Rectangle { color: isPrimary ? "#2979FF" : "#E0E0E0"; radius: 8 }
    }

    // Champ de texte stylisé
    component AppTextField : TextField {
        Layout.fillWidth: true; Layout.preferredHeight: 45
        background: Rectangle { color: "#F5F5F5"; radius: 8; border.width: 1; border.color: parent.activeFocus ? "#2979FF" : "#CCC" }
    }

    // Carte (Bouton large) utilisée sur le menu d'accueil
    component AppCard : Rectangle {
        id: cardRoot
        property string title;
        property string subTitle;
        property string iconChar;
        signal clicked() // Signal émis lors du clic pour déclencher l'action

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
