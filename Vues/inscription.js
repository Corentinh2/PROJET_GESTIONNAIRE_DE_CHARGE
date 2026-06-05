/**
 * @file inscription.js
 * @brief Script de gestion de la page d'inscription (inscription.php).
 *
 * @details
 * Ce fichier gère le formulaire de création de compte utilisateur :
 * - Validation côté client (champs vides, confirmation du mot de passe).
 * - Envoi de la demande de création de compte via AJAX.
 * - Retour visuel via une pastille colorée (`#pastille`).
 * - Redirection automatique vers `login.php` en cas de succès.
 *
 * Dépendances :
 * - jQuery
 * - controleur.php (commande `CreerCompte`)
 *
 * @version 1.0
 */

/**
 * @brief Valide le formulaire d'inscription et crée un compte via AJAX si valide.
 *
 * @details
 * Effectue les vérifications suivantes avant l'envoi :
 * 1. Aucun champ ne doit être vide (`login`, `mdp`, `mdpConfirm`).
 * 2. Les champs `mdp` et `mdpConfirm` doivent être identiques.
 *
 * Si la validation échoue, la pastille `#pastille` passe en avertissement (jaune).
 *
 * En cas d'envoi AJAX, les statuts possibles et leurs effets sont :
 * - `'ok'`     : pastille verte, redirection vers `login.php` après 500 ms.
 * - `'existe'` : pastille jaune (le login est déjà utilisé).
 * - `'erreur'` : pastille rouge (erreur serveur lors de l'insertion).
 *
 * @listens click #btnInscrire
 *
 * @returns {void}
 */
function CreerCompte() {
    var login = $("#login").val();
    var mdp = $("#mdp").val();
    var mdpConfirm = $("#mdpConfirm").val();


    //Si un champ est vide on ne peux pas envoyer la requete.
    if (login == '' || mdp == '' || mdpConfirm == '') {
        $("#pastille").removeClass();
        $("#pastille").addClass("badge text-bg-warning");

    } else {

        // Vérification confirmation mot de passe côté client
        if (mdp !== mdpConfirm) {
            $("#pastille").removeClass();
            $("#pastille").addClass("badge text-bg-warning");

        } else {


            $.ajax({
                url: '../Controleurs/controleur.php',
                data: {
                    "commande": 'CreerCompte',
                    "login": login,
                    "mdp": mdp
                },
                type: 'POST',
                dataType: 'json',
                success: function (donnees) {
                    $("#pastille").removeClass();
                    switch (donnees.statut) {
                        case 'ok':
                            $("#pastille").addClass("badge text-bg-success");
                            setTimeout(function () {
                                window.location.href = "login.php";
                            }, 500);
                            break;
                        case 'existe':
                            $("#pastille").addClass("badge text-bg-warning");
                            break;
                        case 'erreur':
                            $("#pastille").addClass("badge text-bg-danger");
                            break;
                    }
                },
                error: function (xhr, status, error) {
                    console.log("param : " + JSON.stringify(xhr));
                    console.log("status : " + status);
                    console.log("error : " + error);
                }
            });
        }
    }
}

/**
 * @brief Initialise les gestionnaires d'événements au chargement du DOM.
 *
 * @details
 * Attache les événements suivants :
 * - Clic sur `#btnInscrire` → appel de CreerCompte().
 * - Clic sur `#reset` → réinitialise la pastille de statut à son état neutre (gris clair).
 */
$(document).ready(function () {
    $("#btnInscrire").click(CreerCompte);
    $("#reset").click(function () {
        $("#pastille").removeClass();
        $("#pastille").addClass("badge text-bg-light");
    });
});