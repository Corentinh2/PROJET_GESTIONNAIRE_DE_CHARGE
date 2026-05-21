<?php

require_once __DIR__ . '/config.inc.php';

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