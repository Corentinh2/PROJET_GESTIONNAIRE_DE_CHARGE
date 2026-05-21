/**
 * @file SessionsPage.js
 * @brief Fonctions utilitaires pour la page de gestion des sessions de charge.
 * @author Corentin
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Recherche l'index d'une valeur dans un tableau.
 * @param array Tableau dans lequel chercher.
 * @param value Valeur à rechercher.
 * @return Index de la valeur si trouvée, 0 sinon.
 */
function findIndexInArray(array, value) {
    for (var i = 0; i < array.length; i++) {
        if (array[i] === value) { return i; }
    }
    return 0;
}

/**
 * @brief Ajoute ou retire un jour dans la liste des jours sélectionnés.
 * @param tempDaysList Liste actuelle des jours sélectionnés.
 * @param dayStr Jour à ajouter ou retirer.
 * @return Nouvelle liste des jours sélectionnés.
 */
function toggleDay(tempDaysList, dayStr) {
    var list = tempDaysList.slice();
    var idx = list.indexOf(dayStr);
    if (idx !== -1) {
        list.splice(idx, 1);
    } else {
        list.push(dayStr);
    }
    return list;
}

/**
 * @brief Vérifie si un jour est sélectionné.
 * @param tempDaysList Liste des jours sélectionnés.
 * @param dayStr Jour à vérifier.
 * @return true si le jour est sélectionné, false sinon.
 */
function isDaySelected(tempDaysList, dayStr) {
    return tempDaysList.indexOf(dayStr) !== -1;
}

/**
 * @brief Convertit la liste des jours en chaîne de caractères.
 * @param tempDaysList Liste des jours sélectionnés.
 * @return Chaîne des jours séparés par des virgules (ex : "Lun, Mar").
 */
function getDaysString(tempDaysList) {
    return tempDaysList.join(", ");
}

/**
 * @brief Calcule l'heure de fin en ajoutant le suffixe "(lendemain)" si nécessaire.
 * @param startStr Heure de début au format "HH:MM".
 * @param endStr Heure de fin au format "HH:MM".
 * @return Heure de fin avec suffixe si la fin est le lendemain.
 */
function calculerHeureFin(startStr, endStr) {
    var startHour = parseInt(startStr.substring(0, 2));
    var endHour = parseInt(endStr.substring(0, 2));
    var finalEndStr = endStr;
    if (endHour < startHour) {
        finalEndStr = endStr + " (lendemain)";
    }
    return finalEndStr;
}
