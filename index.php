<?php

session_start();
define('BASE_URL', '/');
ini_set('display_errors', 1);
error_reporting(E_ALL);

require_once 'Controleurs/controleur.php';

$action = $_GET['action'] ?? 'Connexion';
$ctrl = new Controleur();

switch ($action) {
    case 'Connexion': $ctrl->Connexion();
        break;
    case 'ChoixParam': $ctrl->ChoixParam();
        break;
    case 'EnregistreGraph': $ctrl->EnregistreGraph();
        break;
    case 'CreerCompte': $ctrl->CreerCompte();
        break;
    case 'GestionUtilisateurs': $ctrl->GestionUtilisateurs();
        break;
    case 'SupprimerUtilisateur': $ctrl->SupprimerUtilisateur();
        break;
    case 'SuspendreUtilisateur': $ctrl->SuspendreUtilisateur();
        break;
    case 'ReactiverUtilisateur': $ctrl->ReactiverUtilisateur();
        break;
    case 'ModifierUtilisateur': $ctrl->ModifierUtilisateur();
        break;
    case 'Deconnexion': $ctrl->Deconnexion();
        break;
    default: echo "Action inconnue.";
        break;
}