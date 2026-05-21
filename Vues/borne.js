function GetMesuresParBorne(idBorne, deb = null, fin = null) {
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
}

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

$(document).ready(function () {
    var params = new URLSearchParams(window.location.search);
    var idBorne = params.get('id');

    if (idBorne) {
        GetNomBorne(idBorne);
        GetMesuresParBorne(idBorne);
        $("#idBorne").val(idBorne);


    }
    $(document).on('click', '#navDeconnexion', Deconnexion);
    $("#btnAfficher").click(function () {
        let deb = $("#dateDebut").val();
        let fin = $("#datefin").val();
        let idBorne=$("#idBorne").val();
        console.log("ici");
        GetMesuresParBorne(idBorne, deb, fin);
    });
    $("#btnRetour").click(function () {
        window.location.href = "choix_bornes.php";
    });

});