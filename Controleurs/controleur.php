<?php

/**
 * @file controleur.php
 * @brief Contrôleur principal de l'application – point d'entrée des requêtes AJAX.
 *
 * Ce fichier reçoit toutes les requêtes HTTP (POST et GET) envoyées par le front-end,
 * identifie la commande demandée et dispatche l'appel vers la fonction de modèle
 * correspondante. Toutes les réponses sont retournées au format JSON.
 *
 * @details
 * Dépendances :
 * - modele.inc.php         : fonctions génériques de connexion à la base de données
 * - modele_utilisateurs.inc.php : CRUD sur les utilisateurs
 * - modele_bornes.inc.php       : CRUD sur les bornes de recharge
 * - modele_mesures.inc.php      : lecture des mesures associées aux bornes
 *
 * @note La session PHP doit être démarrée avant tout appel à ce contrôleur.
 *
 * @version 1.0
 */

session_start();
require_once __DIR__ . '/../Modeles/modele.inc.php';
require_once __DIR__ . '/../Modeles/modele_utilisateurs.inc.php';
require_once __DIR__ . '/../Modeles/modele_bornes.inc.php';
require_once __DIR__ . '/../Modeles/modele_mesures.inc.php';

header('Content-Type: application/json');

/**
 * @section post_handler Traitement des requêtes POST
 *
 * Toutes les commandes ci-dessous sont déclenchées via une requête HTTP POST
 * contenant le paramètre `commande`.
 *
 * | Commande                   | Description                                          |
 * |----------------------------|------------------------------------------------------|
 * | SauveMajIdBorne            | Sauvegarde l'identifiant de borne en session         |
 * | Connexion                  | Authentifie un utilisateur                           |
 * | CreerCompte                | Crée un nouvel utilisateur                           |
 * | Deconnexion                | Détruit la session courante                          |
 * | RecupererListeBorne        | Retourne la liste des bornes (vue publique)          |
 * | GetMesuresParBorne         | Retourne les mesures d'une borne sur une période     |
 * | GetNomBorne                | Retourne le nom de la borne en session               |
 * | RecupererListeUtilisateur  | Retourne la liste des utilisateurs (admin)           |
 * | SupprimerUtilisateur       | Supprime un utilisateur                              |
 * | ModifierUtilisateur        | Modifie le mot de passe d'un utilisateur             |
 * | ActiverUtilisateur         | Active un compte utilisateur                         |
 * | SuspendreUtilisateur       | Suspend un compte utilisateur                        |
 * | GetDernieresMesuresParBorne| Retourne les dernières mesures de la borne en session|
 * | RecupererListeBorneAdmin   | Retourne la liste des bornes (vue admin)             |
 * | AjouterBorne               | Ajoute une nouvelle borne                            |
 * | ModifierBorne              | Modifie les informations d'une borne                 |
 * | SupprimerBorne             | Supprime une borne                                   |
 */
if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') === 'POST') {
    $commande = filter_input(INPUT_POST, 'commande');

    switch ($commande) {

        /**
         * @brief Sauvegarde l'identifiant d'une borne dans la session.
         *
         * @post `$_SESSION['idBorne']` contient l'identifiant validé (entier).
         * @param int id  Identifiant de la borne (POST, validé FILTER_VALIDATE_INT).
         * @return int    L'identifiant de borne tel qu'enregistré en session.
         */
        case 'SauveMajIdBorne':
            $idBorne = filter_input(INPUT_POST, 'id', FILTER_VALIDATE_INT);
            $_SESSION['idBorne'] = $idBorne;
            echo json_encode($idBorne);
            break;

        /**
         * @brief Authentifie un utilisateur avec son login et son mot de passe.
         *
         * @param string login  Identifiant de connexion (POST).
         * @param string mdp    Mot de passe en clair (POST).
         * @return array        `{ "statut": <résultat de Connexion()> }`
         */
        case 'Connexion':
            $login = filter_input(INPUT_POST, 'login');
            $mdp = filter_input(INPUT_POST, 'mdp');
            $reponse = Connexion($login, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Crée un nouveau compte utilisateur.
         *
         * @param string login  Identifiant souhaité (POST).
         * @param string mdp    Mot de passe en clair (POST).
         * @return array        `{ "statut": <résultat de CreerUtilisateur()> }`
         */
        case 'CreerCompte':
            $login = filter_input(INPUT_POST, 'login');
            $mdp = filter_input(INPUT_POST, 'mdp');
            $reponse = CreerUtilisateur($login, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Déconnecte l'utilisateur courant.
         *
         * Délègue à la fonction locale Deconnexion() qui détruit la session.
         *
         * @return array `{ "statut": "ok" }`
         * @see Deconnexion()
         */
        case 'Deconnexion':
            Deconnexion();
            break;

        /**
         * @brief Récupère la liste de toutes les bornes (vue standard).
         *
         * @return array Tableau JSON des bornes retourné par RecupererListeBorne().
         */
        case 'RecupererListeBorne':
            $lstBornes = RecupererListeBorne();
            echo json_encode($lstBornes);
            break;

        /**
         * @brief Retourne les mesures d'une borne pour une période donnée.
         *
         * Si aucune date n'est fournie, la période par défaut couvre les 30 derniers jours.
         *
         * @pre  `$_SESSION['idBorne']` doit être défini (via SauveMajIdBorne).
         * @param string date_debut  Date de début au format `Y-m-d H:i:s` (POST, optionnel).
         * @param string date_fin    Date de fin au format `Y-m-d H:i:s` (POST, optionnel).
         * @return array             Tableau JSON des mesures (nombres encodés numériquement).
         */
        case 'GetMesuresParBorne':
            $idBorne = $_SESSION['idBorne'];
            $debut = filter_input(INPUT_POST, 'date_debut');
            $fin = filter_input(INPUT_POST, 'date_fin');
            $date = new DateTime();
            if ($debut == "" || $fin == "") {
                $debut = $date->format('Y-m-d H:i:s');
                $fin = $date->format('Y-m-d H:i:s');
                $debut = (clone $date)->modify('-30 days')->format('Y-m-d H:i:s');
            }
            echo json_encode(GetMesuresParBorne($idBorne, $debut, $fin), JSON_NUMERIC_CHECK);
            break;

        /**
         * @brief Retourne le nom de la borne actuellement sélectionnée en session.
         *
         * @pre  `$_SESSION['idBorne']` doit être défini.
         * @return string Nom de la borne au format JSON.
         */
        case 'GetNomBorne':
            $idBorne = $_SESSION['idBorne'];
            echo json_encode(GetNomBorne($idBorne));
            break;

        /**
         * @brief Récupère la liste de tous les utilisateurs (usage admin).
         *
         * @return array Tableau JSON des utilisateurs retourné par RecupererListeUtilisateur().
         */
        case 'RecupererListeUtilisateur':
            $lsUtilisateurs = RecupererListeUtilisateur();
            echo json_encode($lsUtilisateurs);
            break;

        /**
         * @brief Supprime un utilisateur par son identifiant.
         *
         * @param int id_utilisateur  Identifiant de l'utilisateur (POST, validé FILTER_VALIDATE_INT).
         * @return array              `{ "statut": <résultat de SupprimerUtilisateur()> }`
         */
        case 'SupprimerUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = SupprimerUtilisateur($id);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Modifie le mot de passe d'un utilisateur existant.
         *
         * @param int    id_utilisateur  Identifiant de l'utilisateur (POST, validé FILTER_VALIDATE_INT).
         * @param string mdp             Nouveau mot de passe en clair (POST).
         * @return array                 `{ "statut": <résultat de ModifierUtilisateur()> }`
         */
        case 'ModifierUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $mdp = filter_input(INPUT_POST, 'mdp');
            $reponse = ModifierUtilisateur($id, $mdp);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Active le compte d'un utilisateur.
         *
         * Appelle ChangerEtatUtilisateur() avec l'état 1 (actif).
         *
         * @param int id_utilisateur  Identifiant de l'utilisateur (POST, validé FILTER_VALIDATE_INT).
         * @return array              `{ "statut": <résultat de ChangerEtatUtilisateur()> }`
         */
        case 'ActiverUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = ChangerEtatUtilisateur($id, 1);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Suspend le compte d'un utilisateur.
         *
         * Appelle ChangerEtatUtilisateur() avec l'état 0 (suspendu).
         *
         * @param int id_utilisateur  Identifiant de l'utilisateur (POST, validé FILTER_VALIDATE_INT).
         * @return array              `{ "statut": <résultat de ChangerEtatUtilisateur()> }`
         */
        case 'SuspendreUtilisateur':
            $id = filter_input(INPUT_POST, 'id_utilisateur', FILTER_VALIDATE_INT);
            $reponse = ChangerEtatUtilisateur($id, 0);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Retourne les dernières mesures enregistrées pour la borne en session.
         *
         * @pre  `$_SESSION['idBorne']` doit être défini.
         * @return array Tableau JSON des mesures les plus récentes (nombres encodés numériquement).
         */
        case 'GetDernieresMesuresParBorne':
            $idBorne = $_SESSION['idBorne'];
            echo json_encode(GetDernieresMesuresParBorne($idBorne), JSON_NUMERIC_CHECK);
            break;

        /**
         * @defgroup admin_bornes Gestion des bornes (admin)
         * @{
         */

        /**
         * @brief Récupère la liste complète des bornes avec les informations d'administration.
         *
         * @return array Tableau JSON des bornes retourné par RecupererListeBorneAdmin().
         */
        case 'RecupererListeBorneAdmin':
            $lstBornes = RecupererListeBorneAdmin();
            echo json_encode($lstBornes);
            break;

        /**
         * @brief Ajoute une nouvelle borne de recharge.
         *
         * Le SSID Wi-Fi et le mot de passe usine sont codés en dur dans le contrôleur
         * (`fortinet` / `Projet2026`).
         *
         * @param string nom_borne    Nom d'affichage de la borne (POST).
         * @param string emplacement  Localisation physique de la borne (POST).
         * @param string puissance    Puissance de charge (POST).
         * @param string ip_usine     Adresse IP d'usine de la borne (POST).
         * @return array              `{ "statut": <résultat de modele_AjoutBorne()> }`
         */
        case 'AjouterBorne':
            $nomBorne    = filter_input(INPUT_POST, 'nom_borne');
            $emplacement = filter_input(INPUT_POST, 'emplacement');
            $puissance   = filter_input(INPUT_POST, 'puissance');
            $ipUsine     = filter_input(INPUT_POST, 'ip_usine');
            $ssidWifi    = 'fortinet';
            $mdpUsine    = 'Projet2026';
            $reponse = modele_AjoutBorne($nomBorne, $emplacement, $puissance, $ipUsine, $ssidWifi, $mdpUsine);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Modifie les informations d'une borne existante.
         *
         * @param int    id_borne     Identifiant de la borne (POST, validé FILTER_VALIDATE_INT).
         * @param string nom_borne    Nouveau nom d'affichage (POST).
         * @param string emplacement  Nouvelle localisation (POST).
         * @param string puissance    Nouvelle puissance (POST).
         * @param string ip_usine     Nouvelle adresse IP d'usine (POST).
         * @return array              `{ "statut": <résultat de ModifierBorne()> }`
         */
        case 'ModifierBorne':
            $idBorne     = filter_input(INPUT_POST, 'id_borne', FILTER_VALIDATE_INT);
            $nomBorne    = filter_input(INPUT_POST, 'nom_borne');
            $emplacement = filter_input(INPUT_POST, 'emplacement');
            $puissance   = filter_input(INPUT_POST, 'puissance');
            $ipUsine     = filter_input(INPUT_POST, 'ip_usine');
            $reponse = ModifierBorne($idBorne, $nomBorne, $emplacement, $puissance, $ipUsine);
            echo json_encode(['statut' => $reponse]);
            break;

        /**
         * @brief Supprime une borne par son identifiant.
         *
         * @param int id_borne  Identifiant de la borne à supprimer (POST, validé FILTER_VALIDATE_INT).
         * @return array        `{ "statut": <résultat de SupprimerBorne()> }`
         */
        case 'SupprimerBorne':
            $idBorne = filter_input(INPUT_POST, 'id_borne', FILTER_VALIDATE_INT);
            $reponse = SupprimerBorne($idBorne);
            echo json_encode(['statut' => $reponse]);
            break;

        /** @} */ // fin du groupe admin_bornes

        /**
         * @brief Commande POST non reconnue.
         *
         * @return string Message d'erreur JSON `"commande inconnue"`.
         */
        default:
            echo json_encode('commande inconnue');
            break;
    }
}

/**
 * @section get_handler Traitement des requêtes GET
 *
 * Commandes disponibles via HTTP GET (paramètre `commande`).
 */
if (filter_input(INPUT_SERVER, 'REQUEST_METHOD') === 'GET') {
    $commande = filter_input(INPUT_GET, 'commande');

    switch ($commande) {

        /**
         * @brief Affiche la page d'accueil (Index).
         *
         * Appelle la fonction Index() définie dans le modèle principal.
         */
        case 'Index':
            Index();
            break;

        /**
         * @brief Commande GET non reconnue.
         *
         * @return string Message d'erreur JSON `"commande inconnue"`.
         */
        default:
            echo json_encode('commande inconnue');
            break;
    }
}

/**
 * @brief Déconnecte l'utilisateur et détruit la session PHP courante.
 *
 * Cette fonction effectue les opérations suivantes :
 * 1. Réinitialise le tableau de session (`$_SESSION = []`).
 * 2. Supprime le cookie de session si la directive `session.use_cookies` est activée.
 * 3. Détruit complètement la session côté serveur.
 *
 * @note Appelée en interne par le `case 'Deconnexion'` du bloc POST.
 *
 * @return void Envoie `{ "statut": "ok" }` en JSON avant de terminer.
 */
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