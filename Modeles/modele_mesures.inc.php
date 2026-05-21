<?php
require_once __DIR__ . '/../Modeles/modele.inc.php';

function GetMesuresParBorne( $idBorne, $debut, $fin) {
    try {
        $connexion=connexionBdd();
        $tabCategories = array();
        $tabSerie      = array();

        $requete = $connexion->prepare("
            SELECT m.horodatage, m.puissance
            FROM MESURES m
            INNER JOIN SESSIONS s ON m.id_session = s.id_session
            WHERE s.id_borne = :idBorne
            AND m.horodatage BETWEEN :debut AND :fin
            ORDER BY m.horodatage ASC
        ");
        $requete->bindParam(":idBorne", $idBorne);
        $requete->bindParam(":debut",   $debut);
        $requete->bindParam(":fin",     $fin);
        $requete->execute();

        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            $tabCategories[] = $ligne['horodatage'];
            $tabSerie[]      = $ligne['puissance'];
        }

        $requete->closeCursor();

        $tabDonnees = array(
            "categories" => $tabCategories,
            "series"     => array(
                array(
                    "name" => "Puissance (kW)",
                    "data" => $tabSerie
                )
            )
        );

        return $tabDonnees;
    } catch (PDOException $erreur) {
        print("Erreur modele_GetMesuresParBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}