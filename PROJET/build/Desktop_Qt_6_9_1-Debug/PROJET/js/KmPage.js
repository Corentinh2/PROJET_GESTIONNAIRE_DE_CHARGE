/**
 * @file KmPage.js
 * @brief Fonctions utilitaires pour la page de validation du kilométrage.
 * @author Corentin
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Récupère le kilométrage actuel d'un véhicule depuis le modèle.
 * @param vName Nom du véhicule recherché.
 * @return Kilométrage actuel du véhicule, 0 si non trouvé.
 */
function getMileage(vName) {
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
 * @brief Met à jour le kilométrage d'un véhicule dans le modèle et sur le serveur.
 * @param kmSaisi Nouveau kilométrage saisi par l'utilisateur.
 */
function validerKilometrage(kmSaisi) {
    for (var v = 0; v < vehiclesModel.count; v++) {
        var vItem = vehiclesModel.get(v);
        if (vItem.name === window.selectedVehicle) {
            vehiclesModel.setProperty(v, "km", kmSaisi.toString());
            commRaspi.modifierKilometrage(vItem.vehicleId, kmSaisi);
        }
    }
}
