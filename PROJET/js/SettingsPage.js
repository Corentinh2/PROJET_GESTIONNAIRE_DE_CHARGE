.pragma library

/**
 * @file SettingsPage.js
 * @brief Fonctions utilitaires pour la page des paramètres.
 * @author Coco
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Convertit et valide le coût saisi par l'utilisateur.
 * @param costText Texte saisi dans le champ coût.
 * @return Valeur numérique valide ou -1 si invalide.
 */
function validerCout(costText) {
    var newCost = parseFloat(costText.replace(",", "."));
    if (!isNaN(newCost) && newCost > 0) {
        return newCost;
    }
    return -1;
}

/**
 * @brief Convertit et valide la puissance saisie par l'utilisateur.
 * @param powerText Texte saisi dans le champ puissance.
 * @return Valeur numérique valide ou -1 si invalide.
 */
function validerPuissance(powerText) {
    var newPower = parseInt(powerText);
    if (!isNaN(newPower) && newPower > 0) {
        return newPower;
    }
    return -1;
}
