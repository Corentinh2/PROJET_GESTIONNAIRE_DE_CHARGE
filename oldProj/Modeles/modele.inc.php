<?php

require_once __DIR__ . '/config.inc.php';

function __construct() {
        try {
            $this->pdo = new PDO(
                    'mysql:host=localhost;dbname=gestionnaire_de_charge;charset=utf8mb4',
                    'charge',
                    'ciel',
                    [
                PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
                PDO::MYSQL_ATTR_INIT_COMMAND => "SET NAMES utf8mb4"
                    ]
            );
            $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        } catch (PDOException $e) {
            error_log("Erreur connexion BDD : " . $e->getMessage());
            die("Erreur de connexion à la base de données.");
        }
    }
