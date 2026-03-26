// logique.js

// L'objet "bookingState" a été supprimé car on utilise window.bookingDays, etc.

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
    window.selectedVehicle = "";

    // CORRECTION : On réinitialise les variables globales
    window.bookingDays = "";
    window.bookingStart = "";
    window.bookingEnd = "";

    return success;
}

function saveSchedule(daysStr, startStr, endStr) {
    var success = true;

    // CORRECTION : On sauvegarde dans le Main.qml
    window.bookingDays = daysStr;
    window.bookingStart = startStr;

    var startHour = parseInt(startStr.substring(0, 2));
    var endHour = parseInt(endStr.substring(0, 2));
    var finalEndStr = endStr;

    if (endHour < startHour) {
        finalEndStr = endStr + " (lendemain)";
    }
    window.bookingEnd = finalEndStr;

    stackView.push(vehiculesPage);

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

    for (var v = 0; v < vehiclesModel.count; v++) {
        var vItem = vehiclesModel.get(v);
        if (vItem.name === window.selectedVehicle) {
            vehiclesModel.setProperty(v, "km", km);
        }
    }

    // CORRECTION : On envoie les variables du window
    commTemp.programmerCharge(
        window.activeStation,
        window.selectedVehicle,
        window.bookingDays,
        window.bookingStart,
        window.bookingEnd
    );

    window.isBookingFlow = false;
    stackView.pop(null);

    found = true;
    return found;
}

function cancelBooking() {
    window.isBookingFlow = false;
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
