// logique.js

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

// 1. CHOIX DE LA BORNE -> Va vers les véhicules
function selectActiveStation(stationName) {
    var success = true;
    window.activeStation = stationName;
    stackView.push(vehiculesPage);
    return success;
}

// 2. CHOIX DU VÉHICULE -> Va vers le kilométrage
function selectVehicle(vehicleName) {
    var success = true;
    window.selectedVehicle = vehicleName;
    stackView.push(mileagePage);
    return success;
}

// 3. VALIDATION DU KM -> Ouvre le Dashboard (Fin du tunnel d'accueil)
function validerDemarrage(km) {
    var success = true;
    window.selectedMileage = km;

    // Mise à jour locale du KM
    for (var v = 0; v < vehiclesModel.count; v++) {
        var vItem = vehiclesModel.get(v);
        if (vItem.name === window.selectedVehicle) {
            vehiclesModel.setProperty(v, "km", km);
        }
    }

    // On remplace tout l'historique de navigation par le Dashboard
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

// --- PARTIE PROGRAMMATION DE CHARGE ---

function startBookingProcess() {
    var success = true;
    window.isBookingFlow = true;

    // On réinitialise uniquement le temps (la borne et le véhicule sont déjà connus)
    window.bookingDays = "";
    window.bookingStart = "";
    window.bookingEnd = "";

    return success;
}

function saveSchedule(daysStr, startStr, endStr) {
    var success = true;
    window.bookingDays = daysStr;
    window.bookingStart = startStr;

    var startHour = parseInt(startStr.substring(0, 2));
    var endHour = parseInt(endStr.substring(0, 2));
    var finalEndStr = endStr;

    if (endHour < startHour) {
        finalEndStr = endStr + " (lendemain)";
    }
    window.bookingEnd = finalEndStr;

    return success;
}

// 4. ENVOI DE LA PROGRAMMATION AU SERVEUR
function finalizeBooking() {
    var found = false;

    // On demande au C++ de générer les trames Raspi et ESP
    commTemp.programmerCharge(
        window.activeStation,
        window.selectedVehicle,
        window.bookingDays,
        window.bookingStart,
        window.bookingEnd
    );

    window.isBookingFlow = false;
    stackView.pop(null); // On revient au tableau de bord

    found = true;
    return found;
}

function cancelBooking() {
    window.isBookingFlow = false;
    stackView.pop(null); // On annule et on revient au tableau de bord
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
