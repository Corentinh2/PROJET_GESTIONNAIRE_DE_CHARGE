<?php

require_once __DIR__ . '/modele.inc.php';

/**
 * @brief Tester si le login est présent dans la table utilisateurs 
 * @param type $login
 * @return vrai si present, faux sinon
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

function CreerUtilisateur($login, $mdp) {
    try {
        $connexion = connexionBdd();
        $resultat = 'erreur';
        if (LoginVerify($login)) {
            $resultat = 'existe';
        } else {
            $requete = $connexion->prepare("INSERT INTO UTILISATEURS (login, mdp, date_creation, etat) VALUES (:login, :mdp, NOW(), 1)");
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

function Connexion($login, $mdp) {
    try {
        $reponse = 'erreur';
        $connexion = connexionBdd();

        $utilisateur = LoginVerify($login);

        if ($utilisateur) {
            $requete = $connexion->prepare("select mdp from UTILISATEURS where login like :login and etat=true;");
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
