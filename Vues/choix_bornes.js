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

function SelectionnerBorne(id) {
    window.location.href = "borne.php?id=" + id;
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
    RecupererListeBorne();
    $(document).on('click', '#navDeconnexion', Deconnexion);
});