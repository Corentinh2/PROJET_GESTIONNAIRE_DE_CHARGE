<?php

class Modele {

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

    public function LoginVerify($login, $password) {
        try {
            $stmt = $this->pdo->prepare("SELECT * FROM UTILISATEUR WHERE login = :login AND etat = 1");
            $stmt->execute([':login' => $login]);
            $user = $stmt->fetch(PDO::FETCH_ASSOC);
            if ($user && password_verify($password, $user['mdp'])) {
                return $user;
            }
            return false;
        } catch (PDOException $e) {
            error_log("Erreur LoginVerify : " . $e->getMessage());
            return false;
        }
    }

    public function UtilisateurExiste($login) {
        try {
            $stmt = $this->pdo->prepare("SELECT COUNT(*) FROM UTILISATEUR WHERE login = :login");
            $stmt->execute([':login' => $login]);
            return $stmt->fetchColumn() > 0;
        } catch (PDOException $e) {
            error_log("Erreur UtilisateurExiste : " . $e->getMessage());
            return false;
        }
    }

    public function CreerUtilisateur($login, $mdp) {
        try {
            $mdp_chiffre = password_hash($mdp, PASSWORD_DEFAULT);
            $stmt = $this->pdo->prepare("INSERT INTO UTILISATEUR (login, mdp, date_creation, etat) VALUES (:login, :mdp, CURDATE(), 1)");
            $stmt->execute([':login' => $login, ':mdp' => $mdp_chiffre]);
            return true;
        } catch (PDOException $e) {
            error_log("Erreur CreerUtilisateur : " . $e->getMessage());
            return false;
        }
    }

    public function RecupererUtilisateurs() {
        try {
            $stmt = $this->pdo->query("SELECT * FROM UTILISATEUR");
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur RecupererUtilisateurs : " . $e->getMessage());
            return [];
        }
    }

    public function RecupererUtilisateurParId($id) {
        try {
            $stmt = $this->pdo->prepare("SELECT * FROM UTILISATEUR WHERE id_utilisateur = :id");
            $stmt->execute([':id' => $id]);
            return $stmt->fetch(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur RecupererUtilisateurParId : " . $e->getMessage());
            return null;
        }
    }

    public function SupprimerUtilisateur($id) {
        try {
            $stmt = $this->pdo->prepare("SELECT id_conducteur FROM CONDUCTEUR WHERE id_utilisateur = :id");
            $stmt->execute([':id' => $id]);
            $conducteurs = $stmt->fetchAll(PDO::FETCH_COLUMN);

            foreach ($conducteurs as $id_conducteur) {
                $stmt = $this->pdo->prepare("SELECT id_vehicule FROM VEHICULE WHERE id_conducteur = :id_conducteur");
                $stmt->execute([':id_conducteur' => $id_conducteur]);
                $vehicules = $stmt->fetchAll(PDO::FETCH_COLUMN);

                foreach ($vehicules as $id_vehicule) {
                    $stmt = $this->pdo->prepare("SELECT id_charge FROM SESSION WHERE id_vehicule = :id_vehicule");
                    $stmt->execute([':id_vehicule' => $id_vehicule]);
                    $sessions = $stmt->fetchAll(PDO::FETCH_COLUMN);

                    foreach ($sessions as $id_charge) {
                        $stmt = $this->pdo->prepare("DELETE FROM MESURE WHERE id_charge = :id_charge");
                        $stmt->execute([':id_charge' => $id_charge]);
                    }

                    $stmt = $this->pdo->prepare("DELETE FROM SESSION WHERE id_vehicule = :id_vehicule");
                    $stmt->execute([':id_vehicule' => $id_vehicule]);
                }

                $stmt = $this->pdo->prepare("DELETE FROM VEHICULE WHERE id_conducteur = :id_conducteur");
                $stmt->execute([':id_conducteur' => $id_conducteur]);
            }

            $stmt = $this->pdo->prepare("DELETE FROM CONDUCTEUR WHERE id_utilisateur = :id");
            $stmt->execute([':id' => $id]);

            $stmt = $this->pdo->prepare("DELETE FROM UTILISATEUR WHERE id_utilisateur = :id");
            $stmt->execute([':id' => $id]);
            return true;
        } catch (PDOException $e) {
            error_log("Erreur SupprimerUtilisateur : " . $e->getMessage());
            return false;
        }
    }

    public function ChangerEtatUtilisateur($id, $etat) {
        try {
            $stmt = $this->pdo->prepare("UPDATE UTILISATEUR SET etat = :etat WHERE id_utilisateur = :id");
            $stmt->execute([':etat' => $etat, ':id' => $id]);
            return true;
        } catch (PDOException $e) {
            error_log("Erreur ChangerEtatUtilisateur : " . $e->getMessage());
            return false;
        }
    }

    public function ModifierUtilisateur($id, $login, $mdp) {
        try {
            $mdp_chiffre = password_hash($mdp, PASSWORD_DEFAULT);
            $stmt = $this->pdo->prepare("UPDATE UTILISATEUR SET login = :login, mdp = :mdp WHERE id_utilisateur = :id");
            $stmt->execute([':login' => $login, ':mdp' => $mdp_chiffre, ':id' => $id]);
            return true;
        } catch (PDOException $e) {
            error_log("Erreur ModifierUtilisateur : " . $e->getMessage());
            return false;
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

    public function GetMesuresParBorne($id_borne, $plage = 'jour') {
        try {
            switch ($plage) {
                case 'semaine': $interval = 'INTERVAL 7 DAY';
                    break;
                case 'mois': $interval = 'INTERVAL 1 MONTH';
                    break;
                default: $interval = 'INTERVAL 1 DAY';
                    break;
            }
            $sql = "
                SELECT 
                    DATE_FORMAT(m.horodatage, '%Y-%m-%d %H:%i') as horodatage,
                    (AVG(m.puissance) / 1000) as puissance
                FROM MESURE m
                JOIN SESSION s ON m.id_charge = s.id_charge
                WHERE s.id_borne = :id_borne
                AND m.horodatage >= NOW() - $interval
                GROUP BY DATE_FORMAT(m.horodatage, '%Y-%m-%d %H'),
                         FLOOR(MINUTE(m.horodatage) / 30)
                ORDER BY horodatage ASC
            ";
            $stmt = $this->pdo->prepare($sql);
            $stmt->execute([':id_borne' => $id_borne]);
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur GetMesuresParBorne : " . $e->getMessage());
            return [];
        }
    }

    public function GetDerniereMesure($id_borne) {
        try {
            $stmt = $this->pdo->prepare("
                SELECT m.horodatage, (m.puissance / 1000) as puissance
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

    public function GetHistorique($id_borne) {
        try {
            $stmt = $this->pdo->prepare("
                SELECT m.horodatage, (m.puissance / 1000) as puissance
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

    public function getUtilisateurParLogin($login) {
        $stmt = $this->pdo->prepare(
                "SELECT * FROM UTILISATEUR WHERE login = :login AND etat = 1 LIMIT 1"
        );
        $stmt->execute([':login' => $login]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }
}
