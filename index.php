<?php
require_once 'controllers/Controleur.php';

$action = $_GET['action'] ?? 'Connexion';
$ctrl = new Controleur();

switch ($action) {
    case 'Connexion':
        $ctrl->Connexion();
        break;
    case 'ChoixParam':
        $ctrl->ChoixParam();
        break;
    case 'EnregistreGraph':
        $ctrl->EnregistreGraph();
        break;
    default:
        $ctrl->Erreur404();
        break;
}