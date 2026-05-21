/* 
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Other/javascript.js to edit this template
 */

function chargerTableau() {
    $.ajax({
        type: 'POST',
        url: '../Controleurs/controleur.php',
        data: {commande: 'RecupererListeUtilisateur'},
        success: function (donnees) {
            $('#table_utilisateur').DataTable({
                data: donnees,
                columns: [
                    {title: "id", name: "id"},
                    {title: "Login", name: "login"},
                    {title: "État", name: "etat"},
                    {title: "Actions"}
                ],
                columnDefs: [
                    {
                        targets: 0,
                        visible: false,
                        searchable: false
                    },
                    {
                        targets: 2,
                        render: function (data) {
                            return data == 1 ? '<span class="badge bg-success">Actif</span>' : '<span class="badge bg-warning text-dark">Suspendu</span>';
                        }
                    },
                    {
                        targets: 3,
                        orderable: false,
                        render: function (data, type, row) {
                            var etat = row[2];
                            var checked;
                            if (etat == 1) {
                                checked = 'checked';
                            } else {
                                checked = '';
                            }
                            var id = row[0];
                            return '<span class="text-primary mod" style="cursor:pointer;" title="Modifier">&#9998;</span>' +
                                    '<span class="text-danger ms-3 supp" style="cursor:pointer;" title="Supprimer">&#128465;</span>' +
                                    '<div class="form-check form-switch d-inline-block ms-3" title="Activer / Suspendre">' +
                                    '<input class="form-check-input toggle-etat" type="checkbox" role="switch" id="switch_' + id + '" ' + checked + ' style="cursor:pointer;">' +
                                    '<label class="form-check-label" for="switch_' + id + '"></label>' +
                                    '</div>';
                        }
                    }
                ],
                paging: true,
                searching: true,
                ordering: true
            });
        },
        error: function (xhr, text, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + text);
            console.log("error : " + error);
        }
    });
}

function modifierUtilisateur() {
    var id = $("#idUtilisateurMod").val();
    var mdp = $("#mdpMod").val();
    var errorContainer = $("#modUtilisateurError");

    errorContainer.addClass("d-none");

    if (mdp !== "" && id !== "") {
        $.ajax({
            type: 'POST',
            url: '../Controleurs/controleur.php',
            data: {commande: 'ModifierUtilisateur', id_utilisateur: id, mdp: mdp},
            success: function (donnees) {
                if (donnees.statut === 'ok') {
                    var modalEl = document.getElementById('modUtilisateurModal');
                    var modal = bootstrap.Modal.getInstance(modalEl);
                    modal.hide();
                    $("#modUtilisateurForm")[0].reset();
                } else {
                    errorContainer.removeClass("d-none");
                    errorContainer.text("Erreur lors de la modification. Veuillez réessayer.");
                }
            },
            error: function (xhr, text, error) {
                console.log("param : " + JSON.stringify(xhr));
                console.log("status : " + text);
                console.log("error : " + error);
            }
        });
    }
}

function supprimerUtilisateur() {
    var id = $("#idUtilisateurSup").val();

    $.ajax({
        type: 'POST',
        url: '../Controleurs/controleur.php',
        data: {commande: 'SupprimerUtilisateur', id_utilisateur: id},
        success: function (donnees) {
            if (donnees.statut === 'ok') {
                var table = $('#table_utilisateur').DataTable();
                var row = table.row(function (idx, data) {
                    return data[0] == id;
                });
                if (row.length) {
                    row.remove().draw();
                }
                var modalEl = document.getElementById('delUtilisateurModal');
                var modal = bootstrap.Modal.getInstance(modalEl);
                modal.hide();
                $("#delUtilisateurForm")[0].reset();
            }
        },
        error: function (xhr, text, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + text);
            console.log("error : " + error);
        }
    });
}

function activerUtilisateur(id) {
    $.ajax({
        type: 'POST',
        url: '../Controleurs/controleur.php',
        data: {commande: 'ActiverUtilisateur', id_utilisateur: id},
        success: function (donnees) {
            if (donnees.statut === 'ok') {
                var table = $('#table_utilisateur').DataTable();
                var row = table.row(function (idx, data) {
                    return data[0] == id;
                });
                if (row.length) {
                    var rowData = row.data();
                    rowData[2] = 1;
                    row.data(rowData).draw(false);
                }
            }
        },
        error: function (xhr, text, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + text);
            console.log("error : " + error);
        }
    });
}

function suspendreUtilisateur(id) {
    $.ajax({
        type: 'POST',
        url: '../Controleurs/controleur.php',
        data: {commande: 'SuspendreUtilisateur', id_utilisateur: id},
        success: function (donnees) {
            if (donnees.statut === 'ok') {
                var table = $('#table_utilisateur').DataTable();
                var row = table.row(function (idx, data) {
                    return data[0] == id;
                });
                if (row.length) {
                    var rowData = row.data();
                    rowData[2] = 0;
                    row.data(rowData).draw(false);
                }
            }
        },
        error: function (xhr, text, error) {
            console.log("param : " + JSON.stringify(xhr));
            console.log("status : " + text);
            console.log("error : " + error);
        }
    });
}

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

$(document).ready(function () {
    chargerTableau();
    $(document).on('click', '#navDeconnexion', Deconnexion);
    // Modifier
    $(document).on('click', '.mod', function () {
        var ligneCliquee = $(this).closest('tr');
        var table = $('#table_utilisateur').DataTable();
        var donneesLigne = table.row(ligneCliquee).data();
        var id = donneesLigne[0];
        var login = donneesLigne[1];
        $("#idUtilisateurMod").val(id);
        $("#loginMod").text(login);
        $("#mdpMod").val('');
        var modal = new bootstrap.Modal(document.getElementById('modUtilisateurModal'));
        modal.show();
    });
    $(document).on('click', '#modif', modifierUtilisateur);

    // Supprimer
    $(document).on('click', '.supp', function () {
        var ligneCliquee = $(this).closest('tr');
        var table = $('#table_utilisateur').DataTable();
        var donneesLigne = table.row(ligneCliquee).data();
        var id = donneesLigne[0];
        var login = donneesLigne[1];
        $("#idUtilisateurSup").val(id);
        $("#supUtilisateurTxt").text(login);
        var modal = new bootstrap.Modal(document.getElementById('delUtilisateurModal'));
        modal.show();
    });
    $(document).on('click', '#delUtilisateur', supprimerUtilisateur);

    // Toggle activer / suspendre
    $(document).on('change', '.toggle-etat', function () {
        var ligneCliquee = $(this).closest('tr');
        var table = $('#table_utilisateur').DataTable();
        var donneesLigne = table.row(ligneCliquee).data();
        var id = donneesLigne[0];
        if ($(this).is(':checked')) {
            activerUtilisateur(id);
        } else {
            suspendreUtilisateur(id);
        }
    });
});