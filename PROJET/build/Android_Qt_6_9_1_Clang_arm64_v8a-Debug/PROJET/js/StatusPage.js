/**
 * @file StatusPage.js
 * @brief Fonctions utilitaires pour la page de suivi en temps réel de la borne.
 * @author Corentin
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Formate un nombre de secondes en chaîne "MM:SS".
 * @param totalSeconds Durée totale en secondes.
 * @return Chaîne formatée (ex : "08:05").
 */
function formaterTemps(totalSeconds) {
    var m = Math.floor(totalSeconds / 60);
    var s = totalSeconds % 60;
    var mm = "";
    var ss = "";
    if (m < 10) { mm = "0" + m; } else { mm = String(m); }
    if (s < 10) { ss = "0" + s; } else { ss = String(s); }
    return mm + ":" + ss;
}

/**
 * @brief Retourne la couleur du texte selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Couleur hexadécimale.
 */
function statusColor(statut) {
    if (statut === "En Charge") { return "#2E7D32"; }
    if (statut === "Programmé") { return "#1565C0"; }
    return "#546E7A";
}

/**
 * @brief Retourne la couleur de fond selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Couleur hexadécimale.
 */
function statusBgColor(statut) {
    if (statut === "En Charge") { return "#E8F5E9"; }
    if (statut === "Programmé") { return "#E3F2FD"; }
    return "#ECEFF1";
}

/**
 * @brief Retourne l'icône selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Caractère emoji représentant le statut.
 */
function statusIcon(statut) {
    if (statut === "En Charge") { return "⚡"; }
    if (statut === "Programmé") { return "📅"; }
    return "🔌";
}

/**
 * @brief Retourne la couleur de bordure selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Couleur hexadécimale.
 */
function statusBorderColor(statut) {
    if (statut === "En Charge") { return "#A5D6A7"; }
    if (statut === "Programmé") { return "#90CAF9"; }
    return "#B0BEC5";
}

/**
 * @brief Retourne le message affiché selon le statut de la borne.
 * @param statut Statut de la borne ("Programmé" ou autre).
 * @return Message descriptif du statut.
 */
function statusMessage(statut) {
    if (statut === "Programmé") { return "Charge programmée"; }
    return "Borne disponible";
}

/**
 * @brief Retourne le texte du bouton selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge" ou autre).
 * @return Texte du bouton d'action.
 */
function buttonText(statut) {
    if (statut === "En Charge") { return "⏹  Arrêter la charge"; }
    return "▶  Lancer la charge";
}

/**
 * @brief Indique si le bouton doit être en mode primaire.
 * @param statut Statut de la borne ("En Charge" ou autre).
 * @return false si en charge, true sinon.
 */
function buttonPrimary(statut) {
    if (statut === "En Charge") { return false; }
    return true;
}

/**
 * @brief Vérifie si la borne active a des sessions programmées.
 * @return true si au moins une session existe pour la borne.
 */
function verifierSessions() {
    var hasSessions = false;
    for (var k = 0; k < sessionsModel.count; k++) {
        if (sessionsModel.get(k).station === window.activeStation) {
            hasSessions = true;
        }
    }
    return hasSessions;
}
