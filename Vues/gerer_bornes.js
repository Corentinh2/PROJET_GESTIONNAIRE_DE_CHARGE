/**
 * @file gerer_bornes.js
 * @brief Script de gestion CRUD des bornes depuis l'interface admin (gerer_bornes.php).
 *
 * @details
 * Ce fichier gère toutes les interactions de la page de gestion des bornes :
 * - Chargement du tableau DataTables listant les bornes.
 * - Ajout d'une nouvelle borne via une modale Bootstrap.
 * - Modification d'une borne existante via une modale Bootstrap.
 * - Suppression d'une borne via une modale de confirmation.
 * - Déconnexion de l'utilisateur.
 *
 * Chaque opération CRUD met à jour le tableau DataTables localement
 * (sans rechargement complet) après confirmation du serveur.
 *
 * Dépendances :
 * - jQuery
 * - DataTables (+ intégration Bootstrap 5)
 * - Bootstrap 5 (modales)
 * - controleur.php (commandes `RecupererListeBorne`, `AjouterBorne`,
 *   `ModifierBorne`, `SupprimerBorne`, `Deconnexion`)
 *
 * @version 1.0
 */

/**
 * @brief Charge la liste des bornes et initialise le tableau DataTables.
 *
 * @details
 * Envoie une requête AJAX POST avec la commande `RecupererListeBorne`.
 * Initialise `#table_borne` avec DataTables en affichant les colonnes :
 * `id_borne` (masquée), `nom_borne`, `emplacement`, `puissance`, `ip_usine`
 * et une colonne Actions avec les boutons Modifier (✏) et Supprimer (🗑).
 *
 * Un bouton `+` dans l'en-tête de la colonne Actions permet d'ouvrir
 * la modale d'ajout (`#addBorneModal`).
 *
 * @returns {void}
 */
function chargerTableauBornes() {
    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'RecupererListeBorne'
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            $('#table_borne').DataTable({
                data: donnees,
                columns: [
                    {title: "id", data: "id_borne"},
                    {title: "Nom", data: "nom_borne"},
                    {title: "Emplacement", data: "emplacement"},
                    {title: "Puissance", data: "puissance"},
                    {title: "IP usine", data: "ip_usine"},
                    {title: "Actions &nbsp;<span class='text-success ms-3' id='ajouterBorne' style='cursor:pointer;'>&#43;</span>"}
                ],
                columnDefs: [
                    {
                        targets: 0,
                        visible: false,
                        searchable: false
                    },
                    {
                        targets: 5,
                        orderable: false,
                        render: function () {
                            return '<span class="text-primary mod" style="cursor:pointer;" title="Modifier">&#9998;</span>' +
                                    '<span class="text-danger ms-3 supp" style="cursor:pointer;" title="Supprimer">&#128465;</span>';
                        }
                    }
                ],
                paging: true,
                searching: true,
                ordering: true
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
 * @brief Ajoute une nouvelle borne à partir des champs de la modale `#addBorneModal`.
 *
 * @details
 * Lit les valeurs des champs `#nomBorneAdd`, `#emplacementAdd`, `#puissanceAdd`
 * et `#ipUsineAdd`. Tous les champs doivent être remplis pour déclencher la requête.
 *
 * En cas de succès (`statut === 'ok'`) :
 * - Ajoute la ligne directement dans le DataTable sans rechargement.
 * - Ferme la modale et réinitialise le formulaire `#addBorneForm`.
 *
 * En cas d'erreur serveur :
 * - Affiche le message d'erreur dans `#addBorneError`.
 *
 * @listens click #ajoutBorne
 *
 * @returns {void}
 */
function ajouterBorne() {
    var nom = $("#nomBorneAdd").val();
    var emplacement = $("#emplacementAdd").val();
    var puissance = $("#puissanceAdd").val();
    var ipUsine = $("#ipUsineAdd").val();
    var errorContainer = $("#addBorneError");

    errorContainer.addClass("d-none");

    if (nom !== "" && emplacement !== "" && puissance !== "" && ipUsine !== "") {
        $.ajax({
            url: '../Controleurs/controleur.php',
            data: {
                "commande": 'AjouterBorne',
                "nom_borne": nom,
                "emplacement": emplacement,
                "puissance": puissance,
                "ip_usine": ipUsine
            },
            type: 'POST',
            dataType: 'json',
            success: function (donnees) {
                if (donnees.statut === 'ok') {
                    var table = $('#table_borne').DataTable();
                    table.row.add({
                        id_borne: null,
                        nom_borne: nom,
                        emplacement: emplacement,
                        puissance: puissance,
                        ip_usine: ipUsine
                    }).draw(false);
                    var modal = bootstrap.Modal.getInstance(document.getElementById('addBorneModal'));
                    modal.hide();
                    $("#addBorneForm")[0].reset();
                } else {
                    errorContainer.removeClass("d-none");
                    errorContainer.text("Erreur lors de l'ajout. Veuillez réessayer.");
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

/**
 * @brief Modifie les informations d'une borne existante à partir de la modale `#modBorneModal`.
 *
 * @details
 * Lit les valeurs des champs `#idBorneMod`, `#nomBorneMod`, `#emplacementMod`,
 * `#puissanceMod` et `#ipUsineMod`. Tous les champs sauf `id_borne` doivent être
 * remplis pour déclencher la requête.
 *
 * En cas de succès (`statut === 'ok'`) :
 * - Met à jour la ligne correspondante dans le DataTable (recherche par `id_borne`).
 * - Ferme la modale et réinitialise le formulaire `#modBorneForm`.
 *
 * En cas d'erreur serveur :
 * - Affiche le message d'erreur dans `#modBorneError`.
 *
 * @listens click #modifBorne
 *
 * @returns {void}
 */
function modifierBorne() {
    var id = $("#idBorneMod").val();
    var nom = $("#nomBorneMod").val();
    var emplacement = $("#emplacementMod").val();
    var puissance = $("#puissanceMod").val();
    var ipUsine = $("#ipUsineMod").val();
    var errorContainer = $("#modBorneError");

    errorContainer.addClass("d-none");

    if (nom !== "" && emplacement !== "" && puissance !== "" && ipUsine !== "") {
        $.ajax({
            url: '../Controleurs/controleur.php',
            data: {
                "commande": 'ModifierBorne',
                "id_borne": id,
                "nom_borne": nom,
                "emplacement": emplacement,
                "puissance": puissance,
                "ip_usine": ipUsine
            },
            type: 'POST',
            dataType: 'json',
            success: function (donnees) {
                if (donnees.statut === 'ok') {
                    var table = $('#table_borne').DataTable();
                    var row = table.row(function (idx, data) {
                        return data.id_borne == id;
                    });
                    if (row.length) {
                        var rowData = row.data();
                        rowData.nom_borne = nom;
                        rowData.emplacement = emplacement;
                        rowData.puissance = puissance;
                        rowData.ip_usine = ipUsine;
                        row.data(rowData).draw(false);
                    }
                    var modal = bootstrap.Modal.getInstance(document.getElementById('modBorneModal'));
                    modal.hide();
                    $("#modBorneForm")[0].reset();
                } else {
                    errorContainer.removeClass("d-none");
                    errorContainer.text("Erreur lors de la modification. Veuillez réessayer.");
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

/**
 * @brief Supprime une borne après confirmation via la modale `#delBorneModal`.
 *
 * @details
 * Lit l'identifiant de la borne dans `#idBorneSup` et envoie une requête AJAX
 * POST avec la commande `SupprimerBorne`.
 *
 * En cas de succès (`statut === 'ok'`) :
 * - Supprime la ligne correspondante du DataTable (recherche par `id_borne`).
 * - Ferme la modale et réinitialise le formulaire `#delBorneForm`.
 *
 * @listens click #delBorne
 *
 * @returns {void}
 */
function supprimerBorne() {
    var id = $("#idBorneSup").val();

    $.ajax({
        url: '../Controleurs/controleur.php',
        data: {
            "commande": 'SupprimerBorne',
            "id_borne": id
        },
        type: 'POST',
        dataType: 'json',
        success: function (donnees) {
            if (donnees.statut === 'ok') {
                var table = $('#table_borne').DataTable();
                var row = table.row(function (idx, data) {
                    return data.id_borne == id;
                });
                if (row.length) {
                    row.remove().draw();
                }
                var modal = bootstrap.Modal.getInstance(document.getElementById('delBorneModal'));
                modal.hide();
                $("#delBorneForm")[0].reset();
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
 * Déclenche le chargement du tableau des bornes et attache les gestionnaires
 * d'événements suivants :
 * - Clic sur `#navDeconnexion`   → Deconnexion().
 * - Clic sur `#ajouterBorne`     → ouverture de la modale d'ajout (`#addBorneModal`).
 * - Clic sur `#ajoutBorne`       → ajouterBorne().
 * - Clic sur `.mod`              → pré-remplissage de la modale de modification et ouverture.
 * - Clic sur `#modifBorne`       → modifierBorne().
 * - Clic sur `.supp`             → pré-remplissage de la modale de suppression et ouverture.
 * - Clic sur `#delBorne`         → supprimerBorne().
 */
$(document).ready(function () {
    chargerTableauBornes();

    $(document).on('click', '#navDeconnexion', Deconnexion);

    // Ajouter
    $(document).on('click', '#ajouterBorne', function () {
        var modal = new bootstrap.Modal(document.getElementById('addBorneModal'));
        modal.show();
    });
    $(document).on('click', '#ajoutBorne', ajouterBorne);

    // Modifier
    $(document).on('click', '.mod', function () {
        var ligneCliquee = $(this).closest('tr');
        var table = $('#table_borne').DataTable();
        var donneesLigne = table.row(ligneCliquee).data();
        $("#idBorneMod").val(donneesLigne.id_borne);
        $("#nomBorneMod").val(donneesLigne.nom_borne);
        $("#emplacementMod").val(donneesLigne.emplacement);
        $("#puissanceMod").val(donneesLigne.puissance);
        $("#ipUsineMod").val(donneesLigne.ip_usine);
        var modal = new bootstrap.Modal(document.getElementById('modBorneModal'));
        modal.show();
    });
    $(document).on('click', '#modifBorne', modifierBorne);

    // Supprimer
    $(document).on('click', '.supp', function () {
        var ligneCliquee = $(this).closest('tr');
        var table = $('#table_borne').DataTable();
        var donneesLigne = table.row(ligneCliquee).data();
        $("#idBorneSup").val(donneesLigne.id_borne);
        $("#supBorneTxt").text(donneesLigne.nom_borne);
        var modal = new bootstrap.Modal(document.getElementById('delBorneModal'));
        modal.show();
    });
    $(document).on('click', '#delBorne', supprimerBorne);
});