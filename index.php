<?php
// index.php à la racine de votre projet NetBeans

require_once 'controllers/LoginController.php';

$action = $_GET['action'] ?? 'showLogin';

$controller = new LoginController();

switch ($action) {
    case 'login':
        $controller->login();
        break;
        
    case 'dashboard':
        echo "Bienvenue sur le tableau de bord !";
        // include 'views/dashboard.php';
        break;

    default:
        $controller->login();
        break;
}