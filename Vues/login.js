/**
 * @file login.js
 * @brief Script de gestion de la page de connexion (login.php).
 *
 * @details
 * Ce fichier gère les interactions utilisateur sur le formulaire de connexion :
 * - Envoi des identifiants au contrôleur via AJAX.
 * - Retour visuel en cas d'erreur (champ rouge).
 * - Redirection après succès.
 * - Déclenchement de la connexion via la touche Entrée.
 *
 * Dépendances :
 * - jQuery
 * - controleur.php (commande `Connexion`)
 *
 * @version 1.0
 */

/**
 * @brief Envoie les identifiants de connexion au contrôleur et traite la réponse.
 *
 * @details
 * Lit les valeurs des champs `#login` et `#mdp`, puis effectue une requête AJAX POST
 * vers le contrôleur avec la commande `Connexion`.
 *
 * Comportement selon le statut retourné :
 * - `'ok'`     : redirige vers `login.php` après 500 ms (le serveur redirigera ensuite
 *                vers `choix_bornes.php` ou `admin.php` selon le rôle).
 * - `'erreur'` : passe le fond du champ `#login` en rouge.
 *
 * @listens click #verifLogin
 * @listens keypress #login, #mdp (touche Entrée)
 *
 * @returns {void}
 */
function Connexion() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'Connexion',
            "login": $("#login").val(),
            "mdp": $("#mdp").val()
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {

            switch (donnees.statut) {
                case 'ok':
                    setTimeout(function () {
                        window.location.href = "login.php";
                    }, 500);
                    break;
                case 'erreur':
                    console.log("erreur");
                    $("#login").css("background-color", "red");

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

/**
 * @brief Initialise les gestionnaires d'événements au chargement du DOM.
 *
 * @details
 * Attache les événements suivants :
 * - Clic sur `#verifLogin` → appel de Connexion().
 * - Pression de la touche Entrée dans `#login` ou `#mdp` → appel de Connexion().
 * - Clic sur `#reset` → réinitialise la pastille de statut à son état neutre.
 */
$(document).ready(function () {
    $("#verifLogin").click(Connexion);
    // Appuyer sur Entrée dans les champs login ou mdp
    $("#login, #mdp").keypress(function (e) {
        if (e.which === 13) {
            Connexion();
        }
    });

    $("#reset").click(function () {
        $("#pastille").removeClass();
        $("#pastille").addClass("badge text-bg-light");
    });
});