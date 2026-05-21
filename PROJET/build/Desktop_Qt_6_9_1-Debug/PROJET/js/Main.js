/**
 * @file Main.js
 * @brief Fonctions utilitaires pour la logique principale de l'application.
 * @author Corentin
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Retourne le statut de la borne actuellement sélectionnée.
 * @return Statut de la borne ("Disponible", "En Charge", "Programmé" ou "Inconnu").
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

/**
 * @brief Compte le nombre de sessions actives pour la borne sélectionnée.
 * @return Nombre de sessions actives.
 */
function compterSessionsActives() {
    var count = 0;
    for (var i = 0; i < sessionsModel.count; i++) {
        if (sessionsModel.get(i).station === window.activeStation) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Calcule l'énergie ajoutée en une seconde selon la puissance maximale.
 * @return Énergie ajoutée en kWh.
 */
function calculerEnergieAjoutee() {
    return window.userMaxPower / 3600.0;
}

/**
 * @brief Calcule le coût estimé de la charge.
 * @return Coût estimé en euros.
 */
function calculerCout() {
    return window.sessionEnergyKwh * window.userCostPerKwh;
}

/**
 * @brief Construit le message d'alerte selon le type reçu.
 * @param type Type d'alerte ("surchauffe" ou autre).
 * @return Message d'alerte formaté.
 */
function construireMessageAlerte(type) {
    var msg = "";
    if (type === "surchauffe") {
        msg = "La charge s'est arrêtée pour la raison suivante : Température trop élevée ! ";
    } else {
        msg = "La charge s'est arrêtée pour la raison suivante : Tension trop élevée !";
    }
    return msg;
}
