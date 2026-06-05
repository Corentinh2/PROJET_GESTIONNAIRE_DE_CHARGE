<?php

/**
 * @file modele_mesures.inc.php
 * @brief Modèle de lecture des mesures de puissance – requêtes sur les tables MESURES et SESSIONS.
 *
 * @details
 * Ce fichier regroupe les fonctions de consultation des mesures de puissance
 * enregistrées par les bornes de recharge. Les mesures sont liées aux bornes
 * via la table SESSIONS (relation MESURES → SESSIONS → BORNES).
 *
 * Fonctions disponibles :
 * - GetMesuresParBorne()         : historique de puissance sur une plage de dates
 * - GetDernieresMesuresParBorne(): les 30 dernières mesures d'une borne
 *
 * Structure des données retournées par GetMesuresParBorne() :
 * @code
 * {
 *   "categories": ["2026-01-01 10:00:00", ...],   // horodatages
 *   "series": [
 *     { "name": "Puissance (kW)", "data": [3.2, 7.1, ...] }
 *   ]
 * }
 * @endcode
 *
 * Dépendances :
 * - modele.inc.php : fonction connexionBdd()
 *
 * @see connexionBdd()
 * @version 1.0
 */
require_once __DIR__ . '/../Modeles/modele.inc.php';

/**
 * @brief Retourne les mesures de puissance d'une borne sur une période donnée.
 *
 * @details
 * Effectue une jointure entre MESURES et SESSIONS pour filtrer les mesures
 * appartenant à la borne identifiée par `$idBorne`, dans l'intervalle
 * `[$debut, $fin]`. Les résultats sont triés par horodatage croissant.
 *
 * La structure retournée est compatible avec les bibliothèques de graphiques
 * (ex. ApexCharts, Highcharts) : un tableau `categories` pour l'axe des abscisses
 * (horodatages) et un tableau `series` pour les données de puissance.
 *
 * @param int    $idBorne  Identifiant de la borne (clé étrangère dans SESSIONS).
 * @param string $debut    Date/heure de début au format `Y-m-d H:i:s`.
 * @param string $fin      Date/heure de fin au format `Y-m-d H:i:s`.
 *
 * @return array Tableau associatif structuré pour le rendu graphique :
 * ```
 * [
 *   "categories" => string[],   // liste des horodatages
 *   "series"     => [
 *     [ "name" => "Puissance (kW)", "data" => float[] ]
 *   ]
 * ]
 * ```
 * Retourne des tableaux vides si aucune mesure ne correspond aux critères.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function GetMesuresParBorne($idBorne, $debut, $fin) {
    try {
        $connexion = connexionBdd();
        $tabCategories = array();
        $tabSerie = array();

        $requete = $connexion->prepare("
            SELECT m.horodatage, m.puissance
            FROM MESURES m
            INNER JOIN SESSIONS s ON m.id_session = s.id_session
            WHERE s.id_borne = :idBorne
            AND m.horodatage BETWEEN :debut AND :fin
            ORDER BY m.horodatage ASC
        ");
        $requete->bindParam(":idBorne", $idBorne);
        $requete->bindParam(":debut", $debut);
        $requete->bindParam(":fin", $fin);
        $requete->execute();

        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            $tabCategories[] = $ligne['horodatage'];
            $tabSerie[] = $ligne['puissance'];
        }

        $requete->closeCursor();

        $tabDonnees = array(
            "categories" => $tabCategories,
            "series" => array(
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

/**
 * @brief Retourne les 30 dernières mesures enregistrées pour une borne.
 *
 * @details
 * Effectue une jointure implicite entre MESURES et SESSIONS, filtrée par
 * l'identifiant de borne. Les résultats sont triés par horodatage décroissant
 * (plus récent en premier) et limités à 30 entrées.
 *
 * Chaque mesure est retournée sous forme de tableau numérique à 4 éléments.
 *
 * @param int $idBorne  Identifiant de la borne dont on veut les dernières mesures.
 *
 * @return array Tableau indexé dont chaque élément est un tableau numérique :
 *               - [0] `id_mesure`   (int)    : identifiant de la mesure
 *               - [1] `horodatage`  (string) : date/heure au format `Y-m-d H:i:s`
 *               - [2] `puissance`   (float)  : valeur de puissance mesurée (kW)
 *               - [3] `id_session`  (int)    : identifiant de la session associée
 *
 * @note Le tri est décroissant (du plus récent au plus ancien).
 *       Le résultat est limité à 30 lignes.
 *
 * @throws PDOException En cas d'erreur SQL (interceptée, message affiché, die()).
 */
function GetDernieresMesuresParBorne($idBorne) {
    try {
        $connexion = connexionBdd();
        $tabMesures = array();
        $requete = $connexion->prepare("
            select id_mesure , horodatage , puissance , SESSIONS.id_session from MESURES, SESSIONS 
            where MESURES.id_session = SESSIONS.id_session 
            and SESSIONS.id_borne = :idBorne 
            order by MESURES.horodatage  desc limit 30;
        ");
        $requete->bindParam(":idBorne", $idBorne);

        $requete->execute();
        while ($ligne = $requete->fetch(PDO::FETCH_ASSOC)) {
            $tabMesures[] = array(
                $ligne['id_mesure'],
                $ligne['horodatage'],
                $ligne['puissance'],
                $ligne['id_session']
            );
        }
        $requete->closeCursor();
        return $tabMesures;
    } catch (PDOException $erreur) {
        print("Erreur GetDernieresMesuresParBorne : " . $erreur->getMessage() . "<br/>");
        die();
    }
}
