/**
 * @file BootPage.js
 * @brief Fonctions utilitaires pour l'affichage de la page de démarrage.
 * @author Corentin
 * @version 1.0
 * @date Avril 2026
 */

/**
 * @brief Retourne la couleur de fond de la bannière d'alerte selon le type.
 * @param type Type d'alerte ("surcharge" ou "temperature").
 * @return Couleur hexadécimale.
 */
function alertTypeColor(type) {
    if (type === "surcharge")   { return "#FFCDD2"; }
    if (type === "temperature") { return "#FFE0B2"; }
    return "#E3F2FD";
}

/**
 * @brief Retourne la couleur de bordure d'une borne selon son statut.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusBorderColor(statut) {
    if (statut === "En Charge") { return "#EF9A9A"; }
    if (statut === "Programmé") { return "#90CAF9"; }
    return "#C8E6C9";
}

/**
 * @brief Retourne la couleur du point de statut d'une borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusDotColor(statut) {
    if (statut === "En Charge") { return "#E53935"; }
    if (statut === "Programmé") { return "#1E88E5"; }
    return "#43A047";
}

/**
 * @brief Retourne le libellé lisible d'un statut de borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Libellé du statut.
 */
function statusLabel(statut) {
    if (statut === "En Charge") { return "En charge"; }
    if (statut === "Programmé") { return "Programmé"; }
    return "Disponible";
}

/**
 * @brief Retourne la couleur de fond de l'icône d'une borne selon son statut.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function iconBgColor(statut) {
    if (statut === "En Charge") { return "#FFEBEE"; }
    if (statut === "Programmé") { return "#E3F2FD"; }
    return "#E8F5E9";
}

/**
 * @brief Retourne la couleur de fond du badge de statut d'une borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusBadgeColor(statut) {
    if (statut === "En Charge") { return "#FFEBEE"; }
    if (statut === "Programmé") { return "#E3F2FD"; }
    return "#E8F5E9";
}
