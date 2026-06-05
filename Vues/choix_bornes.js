/**
 * @file choix_bornes.js
 * @brief Script de gestion de la page de sélection des bornes (choix_bornes.php).
 *
 * @details
 * Ce fichier charge dynamiquement la liste des bornes disponibles et génère
 * des tuiles HTML cliquables dans le conteneur `#listeBornes`.
 * Chaque tuile permet de sélectionner une borne et de naviguer vers sa page de détail.
 *
 * Dépendances :
 * - jQuery
 * - controleur.php (commandes `RecupererListeBorne`, `SauveMajIdBorne`, `Deconnexion`)
 *
 * @version 1.0
 */

/**
 * @brief Récupère la liste des bornes et génère les tuiles d'affichage.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `RecupererListeBorne`.
 * Pour chaque borne reçue, génère une tuile HTML insérée dans `#listeBornes`.
 * La couleur de chaque tuile est cyclée parmi 6 classes CSS (`couleur-0` à `couleur-5`)
 * en fonction de l'index.
 *
 * Chaque tuile affiche :
 * - Le nom de la borne (`nom_borne`)
 * - L'emplacement (`emplacement`)
 * - La puissance (`puissance` en kW)
 *
 * Un clic sur la tuile appelle SelectionnerBorne() avec l'identifiant de la borne.
 *
 * @returns {void}
 */
function RecupererListeBorne() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'RecupererListeBorne'
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            $("#listeBornes").empty();
            $.each(donnees, function (index, borne) {
                var couleur = "couleur-" + (index % 6);
                var tuile = `
                    <div class="col-md-3 col-sm-6">
                        <div class="tuile ${couleur}" onclick="SelectionnerBorne(${borne.id_borne})">
                            <h5>${borne.nom_borne}</h5>
                            <p>📍 ${borne.emplacement}</p>
                            <p>⚡ ${borne.puissance} kW</p>
                        </div>
                    </div>`;
                $("#listeBornes").append(tuile);
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
 * @brief Sauvegarde l'identifiant de la borne sélectionnée en session et redirige vers borne.php.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `SauveMajIdBorne` pour stocker
 * l'identifiant de la borne dans `$_SESSION['idBorne']` côté serveur.
 * En cas de succès, redirige l'utilisateur vers `borne.php`.
 *
 * @param {number} id - Identifiant unique de la borne sélectionnée (`id_borne`).
 *
 * @returns {void}
 */
function SelectionnerBorne(id) {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'SauveMajIdBorne',
            "id": id
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {

            console.log(donnees);
            window.location.href = "borne.php";
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
 * @listens click #btnDeconnexion
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
 * Déclenche le chargement de la liste des bornes et attache
 * le gestionnaire de déconnexion sur `#btnDeconnexion`.
 */
$(document).ready(function () {
    RecupererListeBorne();
    $("#btnDeconnexion").click(Deconnexion);
});