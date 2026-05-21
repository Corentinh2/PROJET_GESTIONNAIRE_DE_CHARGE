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
        <title>Choix de la borne</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="../js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="../js/libs/jquery/jquery.min.js"></script>
        <script src="../js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <script src="choix_bornes.js"></script>
        <style>
            .tuile {
                border-radius: 12px;
                padding: 25px;
                color: white;
                cursor: pointer;
                transition: transform 0.2s, opacity 0.2s;
                min-height: 150px;
                display: flex;
                flex-direction: column;
                justify-content: center;
            }
            .tuile:hover {
                transform: scale(1.03);
                opacity: 0.9;
            }
            .tuile h5 {
                font-size: 1.3rem;
                font-weight: bold;
            }
            .tuile p  {
                font-size: 0.95rem;
                margin: 0;
            }

            .couleur-0 {
                background-color: #4e73df;
            }
            .couleur-1 {
                background-color: #1cc88a;
            }
            .couleur-2 {
                background-color: #36b9cc;
            }
            .couleur-3 {
                background-color: #e74a3b;
            }
            .couleur-4 {
                background-color: #f6c23e;
            }
            .couleur-5 {
                background-color: #858796;
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
                            <a class="nav-link" href="login.php" id="navDeconnexion">Deconnexion</a>
                        </li>
                    </ul>
                </div>
            </div>
        </nav>
    <body>
        <div class="container m-5">
            <h1>Choisissez une borne</h1>
            <div id="listeBornes" class="row g-3"></div>
        </div>

    </body>
</html>