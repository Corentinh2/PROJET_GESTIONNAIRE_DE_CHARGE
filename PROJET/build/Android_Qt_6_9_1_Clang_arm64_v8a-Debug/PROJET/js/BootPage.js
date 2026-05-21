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
    var couleur = "#E3F2FD";
    if (type === "surcharge")   { couleur = "#FFCDD2"; }
    if (type === "temperature") { couleur = "#FFE0B2"; }
    return couleur;
}

/**
 * @brief Retourne la couleur de bordure d'une borne selon son statut.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusBorderColor(statut) {
    var couleur = "#C8E6C9";
    if (statut === "En Charge") { couleur = "#EF9A9A"; }
    if (statut === "Programmé") { couleur = "#90CAF9"; }
    return couleur;
}

/**
 * @brief Retourne la couleur du point de statut d'une borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusDotColor(statut) {
    var couleur = "#43A047";
    if (statut === "En Charge") { couleur = "#E53935"; }
    if (statut === "Programmé") { couleur = "#1E88E5"; }
    return couleur;
}

/**
 * @brief Retourne le libellé lisible d'un statut de borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Libellé du statut.
 */
function statusLabel(statut) {
    var label = "Disponible";
    if (statut === "En Charge") { label = "En charge"; }
    if (statut === "Programmé") { label = "Programmé"; }
    return label;
}

/**
 * @brief Retourne la couleur de fond de l'icône d'une borne selon son statut.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function iconBgColor(statut) {
    var couleur = "#E8F5E9";
    if (statut === "En Charge") { couleur = "#FFEBEE"; }
    if (statut === "Programmé") { couleur = "#E3F2FD"; }
    return couleur;
}

/**
 * @brief Retourne la couleur de fond du badge de statut d'une borne.
 * @param statut Statut de la borne ("En Charge", "Programmé" ou "Disponible").
 * @return Couleur hexadécimale.
 */
function statusBadgeColor(statut) {
    var couleur = "#E8F5E9";
    if (statut === "En Charge") { couleur = "#FFEBEE"; }
    if (statut === "Programmé") { couleur = "#E3F2FD"; }
    return couleur;
}
