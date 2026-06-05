<?php

/**
 * @file modele_utilisateurs.inc.php
 * @brief Modèle de gestion des utilisateurs – opérations CRUD et authentification sur la table UTILISATEURS.
 *
 * @details
 * Ce fichier regroupe toutes les fonctions d'accès aux données relatives aux comptes
 * utilisateurs. Il gère la création, la lecture, la modification, la suppression et
 * l'authentification des utilisateurs via PDO.
 *
 * Fonctions disponibles :
 * - LoginVerify()              : vérifie l'existence et l'activation d'un login
 * - CreerUtilisateur()         : crée un nouveau compte avec hachage du mot de passe
 * - RecupererListeUtilisateur(): liste tous les utilisateurs (usage admin)
 * - SupprimerUtilisateur()     : supprime un utilisateur par son id
 * - ModifierUtilisateur()      : met à jour le mot de passe d'un utilisateur
 * - ChangerEtatUtilisateur()   : active ou suspend un compte utilisateur
 * - Connexion()                : authentifie un utilisateur (login + mot de passe)
 *
 * @note Les mots de passe sont stockés sous forme hachée via password_hash()
 *       et vérifiés via password_verify() (algorithme bcrypt par défaut).
 *
 * Dépendances :
 * - modele.inc.php : fonction connexionBdd()
 *
 * @see connexionBdd()
 * @version 1.0
 */
require_once __DIR__ . '/modele.inc.php';

/**
 * @brief Vérifie si un login est présent et actif dans la table UTILISATEURS.
 *
 * @details
 * Effectue un COUNT sur la table UTILISATEURS en filtrant par login (LIKE)
 * et par état actif (`etat = true`). Retourne `true` si au moins un enregistrement
 * correspond, `false` sinon.
 *
 * @param string $login  Identifiant de connexion à rechercher.
 *
 * @return bool `true` si le login existe et est actif, `false` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function LoginVerify($login) {
    try {
        $connexion = connexionBdd();
        $utilisateur = true;

        $requete = $connexion->prepare("select count(login) as nb from UTILISATEURS "
                . "where login like :login and etat=true;");

        $requete->bindParam(":login", $login);
        $requete->execute();

        $nbLignes = $requete->fetchColumn(0);
        $requete->closeCursor();

        if ($nbLignes == 0) {
            $utilisateur = false;
        }
        return $utilisateur;
    } catch (PDOException $erreur) {
        print("Erreur LoginVerify : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Crée un nouveau compte utilisateur dans la base de données.
 *
 * @details
 * Vérifie d'abord via LoginVerify() que le login n'est pas déjà utilisé.
 * Si le login est disponible, insère un nouvel enregistrement dans UTILISATEURS
 * avec le mot de passe haché (PASSWORD_DEFAULT = bcrypt) et l'état actif (1).
 *
 * @param string $login  Identifiant souhaité pour le nouveau compte.
 * @param string $mdp    Mot de passe en clair (sera haché avant insertion).
 *
 * @return string
 *   - `'ok'`     : compte créé avec succès
 *   - `'existe'` : le login est déjà utilisé par un compte actif
 *   - `'erreur'` : échec de l'insertion SQL
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 *
 * @see LoginVerify()
 */
function CreerUtilisateur($login, $mdp) {
    try {
        $connexion = connexionBdd();
        $resultat = 'erreur';
        if (LoginVerify($login)) {
            $resultat = 'existe';
        } else {
            $requete = $connexion->prepare("INSERT INTO UTILISATEURS (login, mdp, date_creation, etat) "
                    . "VALUES (:login, :mdp, NOW(), 1)");
            $requete->bindParam(":login", $login);
            $mdpHash = password_hash($mdp, PASSWORD_DEFAULT);
            $requete->bindParam(":mdp", $mdpHash);

            if ($requete->execute()) {
                $resultat = 'ok';
            }
        }

        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur CreerUtilisateur : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Récupère la liste de tous les utilisateurs enregistrés (usage admin).
 *
 * @details
 * Retourne un tableau d'entrées contenant pour chaque utilisateur :
 * son identifiant, son login et son état (actif/suspendu).
 * Les mots de passe ne sont jamais inclus dans le résultat.
 *
 * @return array Tableau indexé dont chaque élément est un tableau numérique :
 *               - [0] `id_utilisateur` (int)
 *               - [1] `login`          (string)
 *               - [2] `etat`           (int : 1 = actif, 0 = suspendu)
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function RecupererListeUtilisateur() {
    try {
        $connexion = connexionBdd();
        $requete = $connexion->prepare("SELECT id_utilisateur, login, etat FROM UTILISATEURS");
        $requete->execute();
        $tabUtilisateurs = array();
        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            array_push($tabUtilisateurs, array(
                $ligne['id_utilisateur'],
                $ligne['login'],
                $ligne['etat']
            ));
        }
        $requete->closeCursor();
        return $tabUtilisateurs;
    } catch (PDOException $erreur) {
        print("Erreur RecupererListeUtilisateur : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Supprime un utilisateur de la base de données par son identifiant.
 *
 * @param int $id  Identifiant unique de l'utilisateur à supprimer (`id_utilisateur`).
 *
 * @return string `'ok'` si la suppression a réussi, `'erreur'` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function SupprimerUtilisateur($id) {
    try {
        $connexion = connexionBdd();
        $resultat = 'erreur';
        $requete = $connexion->prepare("DELETE FROM UTILISATEURS WHERE id_utilisateur = :id");
        $requete->bindParam(":id", $id);
        if ($requete->execute()) {
            $resultat = 'ok';
        }
        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur SupprimerUtilisateur : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Modifie le mot de passe d'un utilisateur existant.
 *
 * @details
 * Le nouveau mot de passe est haché via password_hash() (PASSWORD_DEFAULT = bcrypt)
 * avant d'être enregistré dans la base de données.
 *
 * @param int    $id   Identifiant unique de l'utilisateur (`id_utilisateur`).
 * @param string $mdp  Nouveau mot de passe en clair (sera haché avant UPDATE).
 *
 * @return string `'ok'` si la mise à jour a réussi, `'erreur'` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function ModifierUtilisateur($id, $mdp) {
    try {
        $connexion = connexionBdd();
        $resultat = 'erreur';
        $mdpHash = password_hash($mdp, PASSWORD_DEFAULT);
        $requete = $connexion->prepare("UPDATE UTILISATEURS SET mdp = :mdp WHERE id_utilisateur = :id");
        $requete->bindParam(":mdp", $mdpHash);
        $requete->bindParam(":id", $id);
        if ($requete->execute()) {
            $resultat = 'ok';
        }
        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur ModifierUtilisateur : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Active ou suspend un compte utilisateur.
 *
 * @details
 * Met à jour le champ `etat` de l'utilisateur dans UTILISATEURS.
 * Utilisé par les commandes `ActiverUtilisateur` (etat=1) et
 * `SuspendreUtilisateur` (etat=0) du contrôleur.
 *
 * @param int $id    Identifiant unique de l'utilisateur (`id_utilisateur`).
 * @param int $etat  Nouvel état du compte : `1` pour actif, `0` pour suspendu.
 *
 * @return string `'ok'` si la mise à jour a réussi, `'erreur'` sinon.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function ChangerEtatUtilisateur($id, $etat) {
    try {
        $connexion = connexionBdd();
        $resultat = 'erreur';
        $requete = $connexion->prepare("UPDATE UTILISATEURS SET etat = :etat WHERE id_utilisateur = :id");
        $requete->bindParam(":etat", $etat);
        $requete->bindParam(":id", $id);
        if ($requete->execute()) {
            $resultat = 'ok';
        }
        return $resultat;
    } catch (PDOException $erreur) {
        print("Erreur ChangerEtatUtilisateur : " . $erreur->getMessage() . "<br/>");
        die();
    }
}

/**
 * @brief Authentifie un utilisateur par son login et son mot de passe.
 *
 * @details
 * Processus d'authentification en deux étapes :
 * 1. LoginVerify() vérifie que le login existe et que le compte est actif.
 * 2. Le hash du mot de passe stocké en base est comparé au mot de passe fourni
 *    via password_verify().
 *
 * En cas de succès, deux variables de session sont initialisées :
 * - `$_SESSION['connecte']`    = `true`
 * - `$_SESSION['utilisateur']` = login de l'utilisateur
 *
 * @param string $login  Identifiant de connexion.
 * @param string $mdp    Mot de passe en clair à vérifier.
 *
 * @return string
 *   - `'ok'`     : authentification réussie, session initialisée
 *   - `'erreur'` : login inexistant, compte inactif ou mot de passe incorrect
 *
 * @post En cas de succès : `$_SESSION['connecte'] = true` et
 *       `$_SESSION['utilisateur']` contient le login.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 *
 * @see LoginVerify()
 */
function Connexion($login, $mdp) {
    try {
        $reponse = 'erreur';
        $connexion = connexionBdd();

        $utilisateur = LoginVerify($login);

        if ($utilisateur) {
            $requete = $connexion->prepare("select mdp from UTILISATEURS "
                    . "where login like :login and etat=true;");
            $requete->bindParam(":login", $login);
            $requete->execute();
            $nbLignesResult = $requete->rowCount();
            if ($nbLignesResult == 1) {
                $mdpBdd = $requete->fetchColumn(0);
                $mdpOk = password_verify($mdp, $mdpBdd);
                if ($mdpOk) {
                    $_SESSION['connecte'] = true;
                    $_SESSION['utilisateur'] = $login;
                    $reponse = 'ok';
                }
            }
        }

        return $reponse;
    } catch (PDOException $erreur) {
        print("Erreur Connexion : " . $erreur->getMessage() . "<br/>");
        die();
    }
}
