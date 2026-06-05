<?php

/**
 * @file modele.inc.php
 * @brief Couche d'accès à la base de données – fonction de connexion centrale.
 *
 * @details
 * Ce fichier constitue le point d'entrée unique pour l'obtention d'une connexion PDO.
 * Tous les autres modèles doivent inclure ce fichier et appeler connexionBdd() pour
 * interagir avec la base de données MySQL/MariaDB.
 *
 * Dépendances :
 * - config.inc.php : constantes SERVEUR_BDD, NOM_DE_LA_BASE, LOGIN, MOT_DE_PASSE
 *
 * @see config.inc.php
 * @version 1.0
 */
require_once __DIR__ . '/config.inc.php';

/**
 * @brief Crée et retourne une connexion PDO à la base de données.
 *
 * @details
 * La connexion est configurée avec :
 * - Le mode d'erreur PDO::ERRMODE_EXCEPTION pour lever des exceptions en cas d'erreur SQL.
 * - L'encodage UTF-8 (`SET NAMES utf8`) pour garantir la bonne gestion des caractères.
 *
 * Les paramètres de connexion (hôte, base, login, mot de passe) sont lus depuis
 * les constantes définies dans `config.inc.php`.
 *
 * @return PDO Instance PDO prête à l'emploi.
 *
 * @throws PDOException En cas d'échec de connexion au serveur de base de données.
 *                      L'exception est interceptée, un message d'erreur est affiché
 *                      et l'exécution est stoppée via die().
 *
 * @note En cas d'erreur, la fonction affiche le message de l'exception et appelle
 *       die(), ce qui interrompt complètement l'exécution du script.
 */
function connexionBdd() {
    try {
        $dsn = 'mysql:host=' . SERVEUR_BDD . ';dbname=' . NOM_DE_LA_BASE;
        $bdd = new PDO($dsn, LOGIN, MOT_DE_PASSE);
        $bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $bdd->exec("set names utf8");
        return $bdd;
    } catch (PDOException $ex) {
        echo ('</br>Erreur de connexion au serveur BDD : ' . $ex->getMessage());
        die();
    }
}
