<?php

class ModeleMesure {
    
    private $pdo ;
    
    public function __construct() {
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

    public function GetDerniereMesure($id_borne) {
        try {
            $stmt = $this->pdo->prepare("
                SELECT m.horodatage, m.puissance as puissance
                FROM MESURE m
                JOIN SESSION s ON m.id_charge = s.id_charge
                WHERE s.id_borne = :id_borne
                ORDER BY m.horodatage DESC LIMIT 1
            ");
            $stmt->execute([':id_borne' => $id_borne]);
            return $stmt->fetch(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur GetDerniereMesure : " . $e->getMessage());
            return null;
        }
    }
    
    public function GetHistorique($id_borne) {
        try {
            $stmt = $this->pdo->prepare("
                SELECT m.horodatage, m.puissance  as puissance
                FROM MESURE m
                JOIN SESSION s ON m.id_charge = s.id_charge
                WHERE s.id_borne = :id_borne
                ORDER BY m.horodatage DESC
                LIMIT 50
            ");
            $stmt->execute([':id_borne' => $id_borne]);
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur GetHistorique : " . $e->getMessage());
            return [];
        }
    }

    public function GetMesuresParBorne($id_borne, $plage = 'jour', $date_debut = null, $date_fin = null, $mois = null) {
        try {
            switch ($plage) {
                case 'semaine':
                    $sql = "
                    SELECT 
                        DATE_FORMAT(m.horodatage, '%d-%m-%Y') as horodatage,
                        m.puissance  as puissance
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND DATE(m.horodatage) BETWEEN :date_debut AND :date_fin
                    GROUP BY DATE(m.horodatage)
                    ORDER BY DATE(m.horodatage) ASC
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([
                        ':id_borne' => $id_borne,
                        ':date_debut' => $date_debut,
                        ':date_fin' => $date_fin
                    ]);
                    break;

                case 'mois':
                    // inchangé
                    $sql = "
                    SELECT 
                        DATE_FORMAT(m.horodatage, '%d-%m-%Y') as horodatage,
                        m.puissance  as puissance
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND MONTH(m.horodatage) = :mois
                    AND YEAR(m.horodatage) = YEAR(NOW())
                    GROUP BY DATE(m.horodatage)
                    ORDER BY DATE(m.horodatage) ASC
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne, ':mois' => $mois]);
                    break;

                default: // jour — inchangé
                    $sql = "
                    SELECT 
                        DATE_FORMAT(m.horodatage, '%d-%m-%Y %H:%i') as horodatage,
                        m.puissance  as puissance
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND DATE(m.horodatage) = CURDATE()
                    GROUP BY DATE_FORMAT(m.horodatage, '%Y-%m-%d %H'),
                    FLOOR(MINUTE(m.horodatage) / 30)
                    ORDER BY horodatage ASC
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne]);
                    break;
            }
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur GetMesuresParBorne : " . $e->getMessage());
            return [];
        }
    }

}

