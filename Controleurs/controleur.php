<?php

session_start();
require_once __DIR__ . '/../Modeles/modele.inc.php';
require_once __DIR__ . '/../Modeles/modele_utilisateurs.inc.php';
require_once __DIR__ . '/../Modeles/modele_bornes.inc.php';
require_once __DIR__ . '/../Modeles/modele_mesures.inc.php';

header('Content-Type: application/json');

if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') === 'POST') {
    $commande = filter_input(INPUT_POST, 'commande');

    switch ($commande) {

        case 'Connexion':
            $login = filter_input(INPUT_POST, 'login');
            $mdp = filter_input(INPUT_POST, 'mdp');
            $reponse = Connexion($login, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        case 'CreerCompte':
            $login = filter_input(INPUT_POST, 'login');
            $mdp = filter_input(INPUT_POST, 'mdp');
            $reponse = CreerUtilisateur($login, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        case 'Deconnexion':
            Deconnexion();
            break;

        case 'RecupererListeBorne':
            $lstBornes = RecupererListeBorne();
            echo json_encode($lstBornes);
            break;

        case 'GetMesuresParBorne':
            $idBorne = filter_input(INPUT_POST, 'id_borne', FILTER_VALIDATE_INT);
            $debut = filter_input(INPUT_POST, 'date_debut');
            $fin = filter_input(INPUT_POST, 'date_fin');
            $date = new DateTime();
// si l'une des dates est vide
// debut et fin sont la date du jour
            if ($debut == "" || $fin == "") {
                $debut = $date->format('Y-m-d H:i:s');
                $fin = $date->format('Y-m-d H:i:s');
            }
            echo json_encode(GetMesuresParBorne($idBorne, $debut, $fin), JSON_NUMERIC_CHECK);
            break;

        case 'GetNomBorne':
            $idBorne = filter_input(INPUT_POST, 'id_borne', FILTER_VALIDATE_INT);
            echo json_encode(GetNomBorne($idBorne));
            break;

        

        case 'RecupererListeUtilisateur':
            $lsUtilisateurs = RecupererListeUtilisateur();
            echo json_encode($lsUtilisateurs);
            break;

        case 'SupprimerUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = SupprimerUtilisateur($id);
            echo json_encode(['statut' => $reponse]);
            break;

        case 'ModifierUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $login = filter_input(INPUT_POST, 'login');
            $reponse = ModifierUtilisateur($id, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        case 'ActiverUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = ChangerEtatUtilisateur($id, 1);
            echo json_encode(['statut' => $reponse]);
            break;

        case 'SuspendreUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = ChangerEtatUtilisateur($id, 0);
            echo json_encode(['statut' => $reponse]);
            break;

        default:
            echo json_encode('commande inconnue');
            break;
    }
}

if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') === 'GET') {
    $commande = filter_input(INPUT_GET, 'commande');

    switch ($commande) {

        case 'Index':
            Index();
            break;

        default:
            echo json_encode('commande inconnue');
            break;
    }
}

function Deconnexion() {
    $_SESSION = [];

    if (ini_get("session.use_cookies")) {
        $parametres = session_get_cookie_params();
        setcookie(session_name(), '', time() - 42000,
                $parametres["path"], $parametres["domain"]
        );
    }

    session_destroy();
    echo json_encode(['statut' => 'ok']);
}
