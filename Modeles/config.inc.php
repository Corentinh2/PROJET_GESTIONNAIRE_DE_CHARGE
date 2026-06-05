<?php
/**
 * @file config.inc.php
 * @brief Constantes de configuration pour la connexion à la base de données.
 * @details Définit les paramètres d'accès réseau et d'authentification pour le serveur MariaDB/MySQL.
 * @author Maître qui sait tout
 * @date 2026
 */

/** @var string SERVEUR_BDD Adresse IP de l'hôte hébergeant la base de données */
define("SERVEUR_BDD", "172.18.59.133");

/** @var string LOGIN Nom d'utilisateur de la base de données */
define("LOGIN", "charge");

/** @var string MOT_DE_PASSE Mot de passe associé à l'utilisateur de la base de données */
define("MOT_DE_PASSE", "ciel");

/** @var string NOM_DE_LA_BASE Nom de la base de données à cibler */
define("NOM_DE_LA_BASE", "gestionnaire_de_charge");