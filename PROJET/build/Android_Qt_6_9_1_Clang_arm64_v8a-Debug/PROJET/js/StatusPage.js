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
    var couleur = "#546E7A";
    if (statut === "En Charge") { couleur = "#2E7D32"; }
    if (statut === "Programmé") { couleur = "#1565C0"; }
    return couleur;
}

/**
 * @brief Retourne la couleur de fond selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Couleur hexadécimale.
 */
function statusBgColor(statut) {
    var couleur = "#ECEFF1";
    if (statut === "En Charge") { couleur = "#E8F5E9"; }
    if (statut === "Programmé") { couleur = "#E3F2FD"; }
    return couleur;
}

/**
 * @brief Retourne l'icône selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Caractère emoji représentant le statut.
 */
function statusIcon(statut) {
    var icone = "🔌";
    if (statut === "En Charge") { icone = "⚡"; }
    if (statut === "Programmé") { icone = "📅"; }
    return icone;
}

/**
 * @brief Retourne la couleur de bordure selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou autre).
 * @return Couleur hexadécimale.
 */
function statusBorderColor(statut) {
    var couleur = "#B0BEC5";
    if (statut === "En Charge") { couleur = "#A5D6A7"; }
    if (statut === "Programmé") { couleur = "#90CAF9"; }
    return couleur;
}

/**
 * @brief Retourne le message affiché selon le statut de la borne.
 * @param statut Statut de la borne ("Programmé" ou autre).
 * @return Message descriptif du statut.
 */
function statusMessage(statut) {
    var message = "Borne disponible";
    if (statut === "Programmé") { message = "Charge programmée"; }
    return message;
}

/**
 * @brief Retourne le texte du bouton selon le statut de la borne.
 * @param statut Statut de la borne ("En Charge" ou autre).
 * @return Texte du bouton d'action.
 */
function buttonText(statut) {
    var texte = "▶  Lancer la charge";
    if (statut === "En Charge") { texte = "⏹  Arrêter la charge"; }
    return texte;
}

/**
 * @brief Indique si le bouton doit être en mode primaire.
 * @param statut Statut de la borne ("En Charge" ou autre).
 * @return false si en charge, true sinon.
 */
function buttonPrimary(statut) {
    var primary = true;
    if (statut === "En Charge") { primary = false; }
    return primary;
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
