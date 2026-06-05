/**
 * @file mesures.js
 * @brief Script de gestion de la page des 30 dernières mesures (mesures.php).
 *
 * @details
 * Ce fichier gère les interactions de la page d'historique des mesures :
 * - Chargement et affichage des 30 dernières mesures via DataTables.
 * - Affichage du nom de la borne en session dans la navbar et le titre de page.
 * - Déconnexion de l'utilisateur.
 *
 * @note La version précédente des fonctions (commentée) acceptait un paramètre
 *       `idBorne` explicite. La version courante utilise l'identifiant stocké
 *       en session côté serveur (`$_SESSION['idBorne']`).
 *
 * Dépendances :
 * - jQuery
 * - DataTables (+ intégration Bootstrap 5)
 * - controleur.php (commandes `GetDernieresMesuresParBorne`, `GetNomBorne`, `Deconnexion`)
 *
 * @version 1.0
 */

/*
 function GetDernieresMesuresParBorne(idBorne) { ... }  // Version obsolète avec idBorne explicite
 function GetNomBorne(idBorne) { ... }                  // Version obsolète avec idBorne explicite
 */

/**
 * @brief Récupère les 30 dernières mesures de la borne en session et initialise le tableau DataTables.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `GetDernieresMesuresParBorne`.
 * La borne ciblée est déterminée côté serveur via `$_SESSION['idBorne']`.
 *
 * En cas de succès, initialise le tableau `#table_mesures` avec DataTables.
 * Colonnes affichées :
 * - Colonne 0 (`id mesure`)    : masquée et non recherchable.
 * - Colonne 1 (`Horodatage`)   : visible.
 * - Colonne 2 (`Puissance kW`) : visible.
 * - Colonne 3 (`id session`)   : masquée et non recherchable.
 *
 * Le tableau est affiché sans pagination, recherche, tri ni info de lignes.
 *
 * @returns {void}
 */
function GetDernieresMesuresParBorne() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'GetDernieresMesuresParBorne'
                    //    "id_borne": idBorne
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            $('#table_mesures').DataTable({
                data: donnees,
                columns: [
                    {title: "id mesure", name: "id_mesure"},
                    {title: "Horodatage", name: "horodatage"},
                    {title: "Puissance (kW)", name: "puissance"},
                    {title: "id session", name: "id_session"}
                ],
                columnDefs: [
                    {
                        targets: 0,
                        visible: false,
                        searchable: false
                    },
                    {
                        targets: 3,
                        visible: false,
                        searchable: false
                    }
                ],
                paging: false,
                searching: false,
                ordering: false,
                info: false
            });
        },
        error: function (xhr, status, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + status);
            console.log("error : " + error);
        }
    });
}

/**
 * @brief Récupère le nom de la borne en session et met à jour la navbar et le titre de page.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `GetNomBorne` sans paramètre.
 * Le serveur identifie la borne via `$_SESSION['idBorne']`.
 *
 * En cas de succès :
 * - Met à jour `#titreBorne` dans la barre de navigation.
 * - Met à jour `#titreMesures` avec le format : `"30 dernières mesures - <nomBorne>"`.
 *
 * @returns {void}
 */
function GetNomBorne() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'GetNomBorne'
                    //        "id_borne": idBorne
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            $("#titreBorne").text(donnees);
            $("#titreMesures").text("30 dernières mesures - " + donnees);
        },
        error: function (xhr, status, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + status);
            console.log("error : " + error);
        }
    });
}

/**
 * @brief Déconnecte l'utilisateur et redirige vers la page de connexion.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `Deconnexion`.
 * Si le statut retourné est `'ok'`, redirige vers `login.php`.
 *
 * @listens click #navDeconnexion
 *
 * @returns {void}
 */
function Deconnexion() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'Deconnexion'
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            if (donnees.statut === 'ok') {
                window.location.href = "login.php";
            }
        },
        error: function (xhr, status, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + status);
            console.log("error : " + error);
        }
    });
}

/**
 * @brief Initialise la page au chargement du DOM.
 *
 * @details
 * Déclenche le chargement du nom de la borne et du tableau des mesures.
 * Attache le gestionnaire de déconnexion sur `#navDeconnexion`.
 */
$(document).ready(function () {
    /*  var params  = new URLSearchParams(window.location.search);
     var idBorne = params.get('id');
     
     if (idBorne) {
     GetNomBorne(idBorne);
     GetDernieresMesuresParBorne(idBorne);
     }
     */
    GetNomBorne();
    GetDernieresMesuresParBorne();
    $(document).on('click', '#navDeconnexion', Deconnexion);
});