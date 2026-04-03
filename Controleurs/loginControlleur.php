<?php

class LoginController {

    // Méthode principale pour gérer la connexion
    public function login() {
        // 1. Vérifier si le formulaire a été soumis
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {

            $identifiant = $_POST['identifiant'] ?? '';
            $password = $_POST['password'] ?? '';

            // 2. Appel au Modèle (Simulation de vérification)
            // Dans un vrai projet, vous feriez : $user = $this->model->checkAuth($id, $pass);
            if ($this->verifierIdentifiants($identifiant, $password)) {

                // Authentification réussie : on ouvre une session
                session_start();
                $_SESSION['user'] = $identifiant;

                // Redirection vers la page d'accueil ou le tableau de bord
                header('Location: index.php?action=dashboard');
                exit();
            } else {
                // Échec : on prépare un message d'erreur pour la vue
                $erreur = "Identifiant ou mot de passe incorrect.";
                include 'views/login.php';
            }
        } else {
            // Si on arrive sur la page sans POST, on affiche simplement le formulaire
            include 'views/login.php';
        }
    }

    // Fonction temporaire de test (en attendant votre Modèle/Base de données)
    private function verifierIdentifiants($id, $pass) {
        $comptesDeTest = [
            'toto' => '1234',
            'titi' => 'toto' // Inversé selon votre image de test
        ];

        return (isset($comptesDeTest[$id]) && $comptesDeTest[$id] === $pass);
    }
}
