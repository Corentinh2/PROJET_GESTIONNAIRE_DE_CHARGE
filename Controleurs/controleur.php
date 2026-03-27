<?php
require_once 'models/Modele.php';
require_once 'models/Journalisation.php';
require_once 'views/VueClient.php';

class Controleur {
    private $modele;
    private $journalisation;

    public function __construct() {
        $this->modele = new Modele();
        $this->journalisation = new Journalisation();
    }

    public function Connexion() {
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $id = $_POST['identifiant'] ?? '';
            $pass = $_POST['password'] ?? '';
            $user = $this->modele->LoginVerify($id);

            if ($user && ($pass === $user['mot_de_passe'])) {
                if (session_status() == PHP_SESSION_NONE) session_start();
                $_SESSION['user'] = $id;
                $this->journalisation->LogConnexion($id, date('Y-m-d H:i:s'));
                
                // Redirection vers le choix de la borne
                header('Location: index.php?action=ChoixParam');
                exit();
            } else {
                $this->journalisation->LogEchecConnexion($id);
                $erreur = "Identifiant ou mot de passe incorrect.";
                include 'views/login.php';
            }
        } else {
            include 'views/login.php';
        }
    }

    public function ChoixParam() {
        // Cette méthode affiche maintenant la liste des bornes
        include 'views/choix_borne.php';
    }

    public function EnregistreGraph() {
        $data = $this->modele->GetDonnees();
        $vue = new VueClient();
        $vue->DessinerGraphique($data);
    }

    public function Erreur404() {
        echo "Page introuvable.";
    }
}