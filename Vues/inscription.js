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

$(document).ready(function () {
    $("#btnInscrire").click(CreerCompte);
    $("#reset").click(function () {
        $("#pastille").removeClass();
        $("#pastille").addClass("badge text-bg-light");
    });
});