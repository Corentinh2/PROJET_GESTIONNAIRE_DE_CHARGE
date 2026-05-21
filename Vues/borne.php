<?php
session_start();
if (!isset($_SESSION['connecte']) || $_SESSION['connecte'] !== true) {
    header("Location: login.php");
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Détail de la borne</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <script src="https://code.highcharts.com/highcharts.js"></script>
        <script src="borne.js"></script>
        <style>
            label {
                font-family: sans-serif;
                font-size: 1rem;
                padding-right: 10px;
            }
            select {
                font-size: 0.9rem;
                padding: 2px 5px;
            }
        </style>
    </head>
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
            <div class="container-fluid">
                <a class="navbar-brand" href="#">Admin Quiz</a>
                <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarAdmin">
                    <span class="navbar-toggler-icon"></span>
                </button>
                <div class="collapse navbar-collapse" id="navbarAdmin">
                    <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                        <li class="nav-item">
                            <a class="nav-link " href="admin_categories.php" id="navCategories">Catégories</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link active" href="admin_questions.php" id="navQuestions">Questions</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="jeu.php" id="navJeu">Jeu</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="#" id="navDeconnexion">Deconnexion</a>
                        </li>
                    </ul>
                </div>
            </div>
        </nav>
    <body>
        <div class="container m-5">
            <div class="d-flex justify-content-between align-items-center mb-4">
                <h1 id="titreBorne">Chargement...</h1>
                <button class="btn btn-secondary" id="btnRetour">Retour</button>
            </div>
            <form>
                <label for="dateDebut">Choisissez une durée&nbsp;:</label>
                <input type="date" id="dateDebut"/>
                <input type="date" id="datefin"/>
                <input type="button" class="btn btn-secondary" id="btnAfficher" value="Afficher"/>
                <input type="hidden" id="idBorne"/>
            </form>
            <div id="graphe" class="w-100" style="aspect-ratio: 2/1;"></div>
        </div>

    </body>
</html>