/**
 * @file borne.js
 * @brief Script de gestion de la page de détail d'une borne (borne.php).
 *
 * @details
 * Ce fichier gère les interactions de la page de visualisation d'une borne :
 * - Chargement du graphique de consommation via Highcharts.
 * - Récupération et affichage du nom de la borne en session.
 * - Filtrage des mesures par plage de dates.
 * - Déconnexion de l'utilisateur.
 *
 * @note La version précédente des fonctions (commentée) acceptait un paramètre
 *       `idBorne` explicite. La version courante utilise l'identifiant stocké
 *       en session côté serveur (`$_SESSION['idBorne']`).
 *
 * Dépendances :
 * - jQuery
 * - Highcharts (+ modules exporting et export-data)
 * - controleur.php (commandes `GetMesuresParBorne`, `GetNomBorne`, `Deconnexion`)
 *
 * @version 1.0
 */

/*function GetMesuresParBorne(idBorne, deb = null, fin = null) {
 $.ajax({
 url: '../Controleurs/controleur.php',
 data: {
 "commande": 'GetMesuresParBorne',
 "id_borne": idBorne,
 "date_debut": deb,
 "date_fin": fin
 },
 type: 'POST',
 dataType: 'json',
 success: function (donnees) {
 Highcharts.chart('graphe', {
 chart: {
 type: 'column'
 },
 title: {
 text: 'Consommation de la borne'
 },
 exporting: {
 enabled: true,
 buttons: {
 contextButton: {
 menuItems: [
 'downloadPNG',
 'downloadJPEG',
 'downloadPDF',
 'downloadSVG',
 'separator',
 'downloadCSV',
 'downloadXLS'
 ]
 }
 }
 },
 xAxis: {
 categories: donnees.categories,
 title: {
 text: 'Horodatage'
 }
 },
 yAxis: {
 title: {
 text: 'Puissance (kW)'
 }
 },
 series: donnees.series
 });
 },
 error: function (xhr, status, error) {
 console.log("param : " + JSON.stringify(xhr));
 console.log("status : " + status);
 console.log("error : " + error);
 }
 });
 }*/

/**
 * @brief Convertit un horodatage SQL (`YYYY-MM-DD HH:MM:SS`) en format français (`DD/MM/YYYY HH:MM`).
 *
 * @param {string} horodatage - Horodatage au format `YYYY-MM-DD HH:MM:SS`.
 * @returns {string} Horodatage au format `DD/MM/YYYY HH:MM`.
 */
function formatDateFr(horodatage) {
    // horodatage : "2026-01-15 09:30:00"
    let [datePart, timePart] = horodatage.split(' ');
    let [annee, mois, jour] = datePart.split('-');
    let [heure, minute] = timePart.split(':');
    return jour + '/' + mois + '/' + annee + ' ' + heure + ':' + minute;
}

/**
 * @brief Récupère les mesures de la borne en session et affiche le graphique Highcharts.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `GetMesuresParBorne`.
 * La borne ciblée est déterminée côté serveur via `$_SESSION['idBorne']`.
 *
 * En cas de succès :
 * - Génère un graphique en colonnes dans l'élément `#graphe` via Highcharts.
 * - Si des données sont présentes, met à jour automatiquement les champs
 *   `#dateDebut` et `#datefin` avec les dates extrêmes des mesures reçues.
 *
 * @param {string|null} [deb=null] - Date de début au format `YYYY-MM-DD` (optionnel).
 *                                   Si null, le serveur applique la période par défaut (30 jours).
 * @param {string|null} [fin=null] - Date de fin au format `YYYY-MM-DD` (optionnel).
 *
 * @listens click #btnAfficher (via $(document).ready)
 *
 * @returns {void}
 */
function GetMesuresParBorne(deb = null, fin = null) {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'GetMesuresParBorne',
            "date_debut": deb,
            "date_fin": fin
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            // Conversion des horodatages en format français DD/MM/YYYY HH:MM
            let categoriesFr = donnees.categories.map(formatDateFr);

            Highcharts.chart('graphe', {
                chart: {
                    type: 'column'
                },
                title: {
                    text: 'Consommation de la borne'
                },
                xAxis: {
                    categories: categoriesFr,
                    title: {
                        text: 'Horodatage'
                    }
                },
                yAxis: {
                    title: {
                        text: 'Puissance (kW)'
                    }
                },
                series: donnees.series
            });
            if (donnees.categories.length > 0) {
                // Les champs date HTML attendent le format YYYY-MM-DD (format SQL)
                let debut = donnees.categories[0].split(' ')[0];
                let fin = donnees.categories[donnees.categories.length - 1].split(' ')[0];
                $("#dateDebut").val(debut);
                $("#datefin").val(fin);
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
 * @brief Version obsolète — récupère le nom d'une borne par son identifiant explicite.
 *
 * @deprecated Remplacée par la surcharge sans paramètre qui utilise la session serveur.
 *             Conservée en référence. En cas de succès, met à jour `#titreBorne`
 *             avec `donnees.nom`.
 *
 * @param {number} idBorne - Identifiant de la borne à interroger.
 *
 * @returns {void}
 */
function GetNomBorne(idBorne) {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'GetNomBorne',
            "id_borne": idBorne
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            $("#titreBorne").text(donnees.nom);
        },
        error: function (xhr, status, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + status);
            console.log("error : " + error);
        }
    });
}

/**
 * @brief Récupère le nom de la borne stockée en session et l'affiche dans la navbar.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `GetNomBorne` sans paramètre.
 * Le serveur identifie la borne via `$_SESSION['idBorne']`.
 * En cas de succès, met à jour le texte de l'élément `#titreBorne`.
 *
 * @returns {void}
 */
function GetNomBorne() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'GetNomBorne'

        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {

            console.log("nom borne : " + donnees);
            $("#titreBorne").text(donnees);
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
    console.log("deconn");
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
 * Déclenche le chargement du nom de la borne et du graphique par défaut (30 derniers jours).
 * Attache ensuite les gestionnaires d'événements suivants :
 * - Clic sur `#navDeconnexion` → Deconnexion().
 * - Clic sur `#btnAfficher` → GetMesuresParBorne() avec les dates saisies dans
 *   `#dateDebut` et `#datefin`.
 * - Clic sur `#btnRetour` → redirection vers `choix_bornes.php`.
 */
$(document).ready(function () {
//  var params = new URLSearchParams(window.location.search);
//  var idBorne = params.get('id');
    /*
     if (idBorne) {
     GetNomBorne(idBorne);
     GetMesuresParBorne(idBorne);
     $("#idBorne").val(idBorne);
     
     
     }
     * 
     */
    GetNomBorne();
    GetMesuresParBorne();

    $(document).on('click', '#navDeconnexion', Deconnexion);
    $("#btnAfficher").click(function () {
        let deb = $("#dateDebut").val();
        let fin = $("#datefin").val();
        let idBorne = $("#idBorne").val();
        console.log("ici");
        //GetMesuresParBorne(idBorne, deb, fin);
        GetMesuresParBorne(deb, fin);
    });
    $("#btnRetour").click(function () {
        window.location.href = "choix_bornes.php";
    });
});