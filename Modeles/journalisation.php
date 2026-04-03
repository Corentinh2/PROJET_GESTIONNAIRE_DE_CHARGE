<?php

class Journalisation {

    public function LogConnexion($user, $timestamp) {
        // Logique d'enregistrement (ex: table SQL ou fichier error_log)
        error_log("Succès : $user connecté le $timestamp");
    }

    public function LogEchecConnexion($user) {
        error_log("Échec : tentative de connexion pour $user");
    }

    public function CreationUser() {
        // Logique de création
    }
}
