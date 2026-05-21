<?php
require_once __DIR__ . '/../Modeles/modele.inc.php';

function RecupererListeBorne() {
    try {
        $connexion=connexionBdd();
        $bornes  = array();
        $requete = $connexion->query("SELECT id_borne, nom_borne, emplacement, puissance FROM BORNES");

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

function GetNomBorne( $idBorne) {
    try {
        $connexion=connexionBdd();
        $nom     = null;
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

function GetBorneParId($idBorne) {
    try {
        $connexion=connexionBdd();
        $borne   = null;
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

function modele_AjoutBorne( $nomBorne, $emplacement, $puissance, $ipUsine, $ssidWifi, $mdpUsine) {
    try {
        $connexion=connexionBdd();
        $resultat = '';
        $requete  = $connexion->prepare("INSERT INTO BORNES (nom_borne, emplacement, puissance, ip_usine, ssid_wifi, mdp_usine) VALUES (:nomBorne, :emplacement, :puissance, :ipUsine, :ssidWifi, :mdpUsine)");
        $requete->bindParam(":nomBorne",    $nomBorne);
        $requete->bindParam(":emplacement", $emplacement);
        $requete->bindParam(":puissance",   $puissance);
        $requete->bindParam(":ipUsine",     $ipUsine);
        $requete->bindParam(":ssidWifi",    $ssidWifi);
        $requete->bindParam(":mdpUsine",    $mdpUsine);

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

function SupprimerBorne( $idBorne) {
    try {
        $connexion=connexionBdd();
        $resultat = '';
        $requete  = $connexion->prepare("DELETE FROM BORNES WHERE id_borne = :idBorne");
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