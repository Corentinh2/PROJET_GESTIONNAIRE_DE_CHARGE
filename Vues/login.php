<?php
session_start();
if (isset($_SESSION['connecte']) && $_SESSION['connecte'] === true) {
    if ($_SESSION['utilisateur']=='admin'){
        header("Location: admin.php");
    }else
    {
        header("Location: choix_bornes.php");
    }    
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Login</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="../js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet" />
        <script src="../js/libs/jquery/jquery.min.js"></script>
        <script src="login.js"></script>
    </head>
    <body>
        <div class="container m-5">
            <h1>Connexion</h1>
            <div class="row mt-4">
                <div class="col-6">
                    <form id="formulaireLogin">
                        <div class="input-group">
                            <input class="form-control me-3" type="text" id="login" name="login" placeholder="Login" required/>
                            <input class="form-control" type="password" id="mdp" name="mdp" placeholder="Mot de passe" required/>
                        </div>
                        <div class="btn-group mt-3 border border-black w-100">
                            <input class="btn btn-primary me-2" type="button" value="Identification" id="verifLogin"/>
                            <input class="btn btn-primary me-2" type="reset" id="reset"/>
                            <button class="bg-body btn" type="button"> Résultat
                                <span id="pastille" class="badge text-bg-light">&nbsp;&nbsp;</span>
                            </button>
                        </div>
                    </form>
                    <div class="mt-3">
                        <a href="inscription.php" class="btn btn-secondary">S'inscrire</a>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>