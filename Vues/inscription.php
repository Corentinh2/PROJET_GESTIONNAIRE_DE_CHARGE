<?php
session_start();
if (isset($_SESSION['connecte']) && $_SESSION['connecte'] === true) {
    if ($_SESSION['utilisateur']=='admin'){
        header("Location: admin.php");
    } 
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Inscription</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="../js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet" />
        <script src="../js/libs/jquery/jquery.min.js"></script>
        <script src="inscription.js"></script>
    </head>
    <body>
        <div class="container m-5">
            <h1>Inscription</h1>
            <div class="row mt-4">
                <div class="col-6">
                    <form id="formulaireInscription">
                        <div class="mb-3">
                            <input class="form-control" type="text" id="login" placeholder="Login" required/>
                        </div>
                        <div class="mb-3">
                            <input class="form-control" type="password" id="mdp" placeholder="Mot de passe" required/>
                        </div>
                        <div class="mb-3">
                            <input class="form-control" type="password" id="mdpConfirm" placeholder="Confirmer le mot de passe" required/>
                        </div>
                        <div class="btn-group border border-black w-100">
                            <input class="btn btn-success me-2" type="button" value="S'inscrire" id="btnInscrire"/>
                            <input class="btn btn-secondary me-2" type="reset" id="reset"/>
                            <div class="bg-body btn" type="button"> Résultat
                                <span id="pastille" class="badge text-bg-light">&nbsp;&nbsp;</span>
                            </div>
                        </div>
                    </form>
                    <div class="mt-3">
                        <a href="login.php" class="btn btn-link">Déjà un compte ? Se connecter</a>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>