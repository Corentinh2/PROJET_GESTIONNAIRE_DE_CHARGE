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
                    setTimeout(function(){
                        window.location.href = "login.php";
                    }, 500);
                    break;
                case 'erreur':
                    console.log("erreur");
                  $("#login").css("background-color","red");  
                  
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

$(document).ready(function () {
    $("#verifLogin").click(Connexion);
    $("#reset").click(function () {
        $("#pastille").removeClass();
        $("#pastille").addClass("badge text-bg-light");
    });
});