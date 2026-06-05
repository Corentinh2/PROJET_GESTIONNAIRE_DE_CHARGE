<?php

/**
 * @file modele_bornes.inc.php
 * @brief Modèle de gestion des bornes de recharge – opérations CRUD sur la table BORNES.
 *
 * @details
 * Ce fichier regroupe toutes les fonctions d'accès aux données relatives aux bornes
 * de recharge électrique. Il dialogue avec la table `BORNES` de la base de données
 * via PDO.
 *
 * Fonctions disponibles :
 * - RecupererListeBorne()   : liste publique des bornes
 * - GetNomBorne()           : nom d'une borne par son identifiant
 * - GetBorneParId()         : tous les champs d'une borne par son identifiant
 * - modele_AjoutBorne()     : insertion d'une nouvelle borne
 * - SupprimerBorne()        : suppression d'une borne par son identifiant
 *
 * @note Ce fichier ne contient pas de fonction ModifierBorne() ; celle-ci doit être
 *       définie ailleurs ou ajoutée si nécessaire.
 *
 * Dépendances :
 * - modele.inc.php : fonction connexionBdd()
 *
 * @see connexionBdd()
 * @version 1.0
 */

require_once __DIR__ . '/../Modeles/modele.inc.php';

/**
 * @brief Récupère la liste de toutes les bornes (vue publique).
 *
 * @details
 * Exécute un SELECT sur la table BORNES et retourne les colonnes
 * `id_borne`, `nom_borne`, `emplacement`, `puissance` et `ip_usine`.
 *
 * @return array Tableau associatif indexé contenant toutes les bornes.
 *               Chaque élément est un tableau associatif avec les clés :
 *               - `id_borne`    (int)
 *               - `nom_borne`   (string)
 *               - `emplacement` (string)
 *               - `puissance`   (string)
 *               - `ip_usine`    (string)
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function RecupererListeBorne() {
    try {
        $connexion = connexionBdd();
        $bornes = array();
        $requete = $connexion->query("SELECT id_borne, nom_borne, emplacement, puissance, ip_usine  FROM BORNES");

        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            $bornes[] = $ligne;
        }

        $requete->closeCursor();
        return $bornes;
    } catch (PDOException $erreur) {
        print("Erreur modele_RecupererListeBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Retourne le nom d'une borne à partir de son identifiant.
 *
 * @param int $idBorne  Identifiant unique de la borne (clé primaire de BORNES).
 *
 * @return string|null  Nom de la borne (`nom_borne`) si elle existe, `null` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function GetNomBorne($idBorne) {
    try {
        $connexion = connexionBdd();
        $nom = null;
        $requete = $connexion->prepare("SELECT nom_borne FROM BORNES WHERE id_borne = :idBorne");
        $requete->bindParam(":idBorne", $idBorne);
        $requete->execute();

        $ligne = $requete->fetch(PDO::FETCH_ASSOC);
        $requete->closeCursor();

        if ($ligne) {
            $nom = $ligne['nom_borne'];
        }

        return $nom;
    } catch (PDOException $erreur) {
        print("Erreur modele_GetNomBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Retourne l'ensemble des informations d'une borne à partir de son identifiant.
 *
 * @details
 * Effectue un `SELECT *` sur la table BORNES pour la borne demandée.
 * Retourne toutes les colonnes disponibles sous forme de tableau associatif.
 *
 * @param int $idBorne  Identifiant unique de la borne (clé primaire de BORNES).
 *
 * @return array|null   Tableau associatif de la borne si elle existe, `null` sinon.
 *                      Les clés correspondent aux colonnes de la table BORNES.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function GetBorneParId($idBorne) {
    try {
        $connexion = connexionBdd();
        $borne = null;
        $requete = $connexion->prepare("SELECT * FROM BORNES WHERE id_borne = :idBorne");
        $requete->bindParam(":idBorne", $idBorne);
        $requete->execute();

        $ligne = $requete->fetch(PDO::FETCH_ASSOC);
        $requete->closeCursor();

        if ($ligne) {
            $borne = $ligne;
        }

        return $borne;
    } catch (PDOException $erreur) {
        print("Erreur modele_GetBorneParId : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Insère une nouvelle borne de recharge dans la base de données.
 *
 * @details
 * Effectue un INSERT dans la table BORNES avec les six paramètres fournis.
 * Le SSID Wi-Fi et le mot de passe usine sont typiquement fournis par le contrôleur
 * avec des valeurs fixes (ex. `fortinet` / `Projet2026`).
 *
 * @param string $nomBorne    Nom d'affichage de la borne.
 * @param string $emplacement Localisation physique de la borne.
 * @param string $puissance   Puissance maximale de charge (ex. "22kW").
 * @param string $ipUsine     Adresse IP d'usine de la borne.
 * @param string $ssidWifi    SSID du réseau Wi-Fi auquel la borne est rattachée.
 * @param string $mdpUsine    Mot de passe usine de la borne.
 *
 * @return string `'ok'` si l'insertion a réussi, `'erreur'` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function modele_AjoutBorne($nomBorne, $emplacement, $puissance, $ipUsine, $ssidWifi, $mdpUsine) {
    try {
        $connexion = connexionBdd();
        $resultat = '';
        $requete = $connexion->prepare("INSERT INTO BORNES (nom_borne, emplacement, puissance, ip_usine, ssid_wifi, mdp_usine) "
                . "VALUES (:nomBorne, :emplacement, :puissance, :ipUsine, :ssidWifi, :mdpUsine)");
        $requete->bindParam(":nomBorne", $nomBorne);
        $requete->bindParam(":emplacement", $emplacement);
        $requete->bindParam(":puissance", $puissance);
        $requete->bindParam(":ipUsine", $ipUsine);
        $requete->bindParam(":ssidWifi", $ssidWifi);
        $requete->bindParam(":mdpUsine", $mdpUsine);

        if ($requete->execute()) {
            $resultat = 'ok';
        } else {
            $resultat = 'erreur';
        }

        $requete->closeCursor();
        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur modele_AjoutBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Supprime une borne de la base de données par son identifiant.
 *
 * @details
 * Effectue un DELETE sur la table BORNES pour la borne dont l'identifiant
 * correspond à `$idBorne`.
 *
 * @warning La suppression d'une borne peut entraîner des suppressions en cascade
 *          si des contraintes de clé étrangère sont définies sur les tables liées
 *          (ex. SESSIONS, MESURES).
 *
 * @param int $idBorne  Identifiant unique de la borne à supprimer.
 *
 * @return string `'ok'` si la suppression a réussi, `'erreur'` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function SupprimerBorne($idBorne) {
    try {
        $connexion = connexionBdd();
        $resultat = '';
        $requete = $connexion->prepare("DELETE FROM BORNES WHERE id_borne = :idBorne");
        $requete->bindParam(":idBorne", $idBorne);

        if ($requete->execute()) {
            $resultat = 'ok';
        } else {
            $resultat = 'erreur';
        }

        $requete->closeCursor();
        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur modele_SupprimerBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}