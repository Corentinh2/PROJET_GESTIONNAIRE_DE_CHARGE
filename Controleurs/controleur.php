<?php

require_once __DIR__ . '/../Modeles/modele.php';
require_once __DIR__ . '/../Modeles/journalisation.php';
require_once __DIR__ . '/../Vues/VueClient.php';

// Identifiants admin fixes
define('ADMIN_LOGIN', 'admin');
define('ADMIN_MDP', 'admin1234');

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

            // Vérification admin
            if ($id === ADMIN_LOGIN && $pass === ADMIN_MDP) {
                $_SESSION['user'] = $id;
                $_SESSION['role'] = 'admin';
                header('Location: http://172.18.59.133/index.php?action=GestionUtilisateurs');
                exit();
            }

            // LoginVerify vérifie maintenant login ET mot de passe ensemble
            $user = $this->modele->LoginVerify($id);

            if ($user) {
                $_SESSION['user'] = $id;
                $_SESSION['role'] = 'user';
                $this->journalisation->LogConnexion($id, date('Y-m-d H:i:s'));
                header('Location: http://172.18.59.133/index.php?action=ChoixParam');
                exit();
            } else {
                if (!$this->modele->UtilisateurExiste($id)) {
                    header('Location: http://172.18.59.133/index.php?action=CreerCompte&identifiant=' . urlencode($id));
                    exit();
                }
                $erreur = "Mot de passe incorrect.";
                include __DIR__ . '/../Vues/login.php';
            }
        } else {
            include __DIR__ . '/../Vues/login.php';
        }
    }

    public function GestionUtilisateurs() {
        // Seul l'admin peut accéder
        if (!isset($_SESSION['role']) || $_SESSION['role'] !== 'admin') {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $listeUtilisateurs = $this->modele->RecupererUtilisateurs();
        include __DIR__ . '/../Vues/gestion_utilisateurs.php';
    }

    public function SupprimerUtilisateur() {
        if (!isset($_SESSION['role']) || $_SESSION['role'] !== 'admin') {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $id = $_GET['id'] ?? null;
        if ($id)
            $this->modele->SupprimerUtilisateur($id);
        header('Location: http://172.18.59.133/index.php?action=GestionUtilisateurs');
        exit();
    }

    public function SuspendreUtilisateur() {
        if (!isset($_SESSION['role']) || $_SESSION['role'] !== 'admin') {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $id = $_GET['id'] ?? null;
        if ($id)
            $this->modele->ChangerEtatUtilisateur($id, 0);
        header('Location: http://172.18.59.133/index.php?action=GestionUtilisateurs');
        exit();
    }

    public function ReactiverUtilisateur() {
        if (!isset($_SESSION['role']) || $_SESSION['role'] !== 'admin') {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $id = $_GET['id'] ?? null;
        if ($id)
            $this->modele->ChangerEtatUtilisateur($id, 1);
        header('Location: http://172.18.59.133/index.php?action=GestionUtilisateurs');
        exit();
    }

    public function ModifierUtilisateur() {
        if (!isset($_SESSION['role']) || $_SESSION['role'] !== 'admin') {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $id = $_POST['id'] ?? null;
            $login = $_POST['login'] ?? '';
            $mdp = $_POST['mdp'] ?? '';
            if ($id)
                $this->modele->ModifierUtilisateur($id, $login, $mdp);
            header('Location: http://172.18.59.133/index.php?action=GestionUtilisateurs');
            exit();
        }
        $id = $_GET['id'] ?? null;
        $utilisateur = $id ? $this->modele->RecupererUtilisateurParId($id) : null;
        include __DIR__ . '/../Vues/modifier_utilisateur.php';
    }

    public function CreerCompte() {
        $identifiant = $_GET['identifiant'] ?? '';
        if ($_SERVER['REQUEST_METHOD'] === 'POST') {
            $login = $_POST['identifiant'] ?? '';
            $mdp = $_POST['password'] ?? '';
            $mdp_confirm = $_POST['password_confirm'] ?? '';
            if ($mdp !== $mdp_confirm) {
                $erreur = "Les mots de passe ne correspondent pas.";
                include __DIR__ . '/../Vues/creer_compte.php';
                return;
            }
            if ($this->modele->UtilisateurExiste($login)) {
                $erreur = "Cet identifiant existe déjà.";
                include __DIR__ . '/../Vues/creer_compte.php';
                return;
            }
            $this->modele->CreerUtilisateur($login, $mdp);
            $succes = "Compte créé ! Vous pouvez vous connecter.";
            include __DIR__ . '/../Vues/login.php';
        } else {
            include __DIR__ . '/../Vues/creer_compte.php';
        }
    }

    public function ChoixParam() {
        if (!isset($_SESSION['user'])) {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $listeBornes = $this->modele->RecupererListeBorne();
        include __DIR__ . '/../Vues/choix_borne.php';
    }

    public function EnregistreGraph() {
        if (!isset($_SESSION['user'])) {
            header('Location: http://172.18.59.133/index.php?action=Connexion');
            exit();
        }
        $id_borne = $_GET['id'] ?? 1;
        $plage = $_GET['plage'] ?? 'jour';
        $mesures = $this->modele->GetMesuresParBorne($id_borne, $plage);
        include __DIR__ . '/../Vues/graphique.php';
    }

    public function Deconnexion() {
        session_destroy();
        header('Location: http://172.18.59.133/index.php?action=Connexion');
        exit();
    }
}
