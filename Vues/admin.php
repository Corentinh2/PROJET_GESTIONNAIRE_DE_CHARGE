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
        <title>Gestion utilisateur</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <link href="js/libs/datatables/css/dataTables.bootstrap5.min.css" rel="stylesheet"/>
        <script src="js/libs/datatables/js/jquery.dataTables.min.js"></script>
        <script src="js/libs/datatables/js/dataTables.bootstrap5.min.js"></script>
        <script src="admin.js"></script>
        <style>
            .mod  {
                cursor: pointer;
            }
            .supp {
                cursor: pointer;
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
        <div class="container mt-5">
            <h2>Gestion des Utilisateurs</h2>
            <table id="table_utilisateur" class="table table-striped table-bordered text-center">
                <thead class="table-dark">
                    <tr>
                        <th class="text-center">id</th>
                        <th class="text-center">Login</th>
                        <th class="text-center">État</th>
                        <th class="text-center">Actions</th>
                    </tr>
                </thead>
                <tbody></tbody>
            </table>
        </div>

        <!-- Modal Modifier un utilisateur -->
        <div class="modal fade" id="modUtilisateurModal" tabindex="-1" aria-labelledby="modUtilisateurModalLabel" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="modUtilisateurModalLabel">Modifier le mot de passe</h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                    </div>
                    <div class="modal-body">
                        <form id="modUtilisateurForm">
                            <div class="mb-3">
                                <label class="form-label">Login</label>
                                <p class="form-control-plaintext fw-bold" id="loginMod"></p>
                            </div>
                            <div class="mb-3">
                                <label for="mdpMod" class="form-label">Nouveau mot de passe</label>
                                <input type="password" class="form-control" id="mdpMod" required>
                            </div>
                            <input type="hidden" id="idUtilisateurMod"/>
                        </form>
                        <div id="modUtilisateurError" class="alert alert-danger d-none"></div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                        <button type="button" class="btn btn-primary" id="modif">Mettre à jour</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- Modal Supprimer un utilisateur -->
        <div class="modal fade" id="delUtilisateurModal" tabindex="-1" aria-labelledby="delUtilisateurModalLabel" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="delUtilisateurModalLabel">Supprimer un utilisateur</h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                    </div>
                    <div class="modal-body">
                        <form id="delUtilisateurForm">
                            <div class="mb-3">
                                Êtes-vous sûr de vouloir supprimer l'utilisateur <b id="supUtilisateurTxt"></b> ?
                            </div>
                            <input type="hidden" id="idUtilisateurSup"/>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                        <button type="button" class="btn btn-danger" id="delUtilisateur">Supprimer</button>
                    </div>
                </div>
            </div>
        </div>

    </body>
</html>