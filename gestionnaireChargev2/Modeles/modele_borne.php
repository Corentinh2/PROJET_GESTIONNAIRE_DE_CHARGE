<?php

class ModeleBorne {

    private $pdo;

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

    public function RecupererListeBorne() {
        try {
            $stmt = $this->pdo->query("SELECT id_borne, nom_borne, emplacement, puissance FROM BORNE");
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur RecupererListeBorne : " . $e->getMessage());
            return [];
        }
    }

    
    public function GetNomBorne($id_borne) {
        try {
            $stmt = $this->pdo->prepare("SELECT nom_borne FROM BORNE WHERE id_borne = :id_borne");
            $stmt->execute([':id_borne' => $id_borne]);
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
            return $result ? $result['nom_borne'] : '';
        } catch (PDOException $e) {
            error_log("Erreur GetNomBorne : " . $e->getMessage());
            return '';
        }
    }
    
    public function AjouterBorne($nom, $emplacement, $puissance, $ip) {
        try {
            $stmt = $this->pdo->prepare("
            INSERT INTO BORNE (nom_borne, emplacement, puissance, ip_usine, ssid_wifi, mdp_usine) 
            VALUES (:nom, :emplacement, :puissance, :ip, 'fortinet', 'Projet2026')
        ");
            $stmt->execute([
                ':nom' => $nom,
                ':emplacement' => $emplacement,
                ':puissance' => $puissance,
                ':ip' => $ip
            ]);
            return true;
        } catch (PDOException $e) {
            error_log("Erreur AjouterBorne : " . $e->getMessage());
            return false;
        }
    }
    
    public function SupprimerBorne($id_borne) {
        try {
            // 1. Récupérer les id_charge liés à cette borne
            $stmt = $this->pdo->prepare("SELECT id_charge FROM SESSION WHERE id_borne = :id_borne");
            $stmt->execute([':id_borne' => $id_borne]);
            $sessions = $stmt->fetchAll(PDO::FETCH_COLUMN);

            // 2. Supprimer les MESURE liées à ces sessions
            foreach ($sessions as $id_charge) {
                $stmt = $this->pdo->prepare("DELETE FROM MESURE WHERE id_charge = :id_charge");
                $stmt->execute([':id_charge' => $id_charge]);
            }

            // 3. Supprimer les SESSION liées à cette borne
            $stmt = $this->pdo->prepare("DELETE FROM SESSION WHERE id_borne = :id_borne");
            $stmt->execute([':id_borne' => $id_borne]);

            // 4. Supprimer la BORNE
            $stmt = $this->pdo->prepare("DELETE FROM BORNE WHERE id_borne = :id_borne");
            $stmt->execute([':id_borne' => $id_borne]);

            return true;
        } catch (PDOException $e) {
            error_log("Erreur SupprimerBorne : " . $e->getMessage());
            return false;
        }
    }
}
