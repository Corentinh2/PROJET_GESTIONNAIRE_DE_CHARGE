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
            // Plus besoin de supprimer les véhicules/sessions/mesures
            // car les véhicules ne sont plus liés aux utilisateurs
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

    public function GetMesuresParBorne($id_borne, $plage = 'jour', $semaine = null, $mois = null) {
        try {
            switch ($plage) {
                case 'semaine':
                    $sql = "
                    SELECT 
                    DATE_FORMAT(m.horodatage, '%d-%m-%Y') as horodatage,
                    AVG(m.puissance) as puissance
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND WEEK(m.horodatage, 1) = :semaine
                    AND YEAR(m.horodatage) = YEAR(NOW())
                    GROUP BY DATE(m.horodatage)
                    ORDER BY DATE(m.horodatage) ASC
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne, ':semaine' => $semaine]);
                    break;
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne, ':semaine' => $semaine]);
                    break;

                case 'mois':
                    $sql = "
                     SELECT 
                    DATE_FORMAT(m.horodatage, '%d-%m-%Y') as horodatage,
                    AVG(m.puissance) as puissance
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

                default: // jour
                    $sql = "
                    SELECT 
                    DATE_FORMAT(m.horodatage, '%d-%m-%Y %H:%i') as horodatage,
                    AVG(m.puissance) as puissance
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

   /* public function GetEvenements($id_borne, $plage = 'jour', $semaine = null, $mois = null) {
        try {
            switch ($plage) {
                case 'semaine':
                    $where = "AND WEEK(e.horodatage, 1) = :semaine AND YEAR(e.horodatage) = YEAR(NOW())";
                    break;
                case 'mois':
                    $where = "AND MONTH(e.horodatage) = :mois AND YEAR(e.horodatage) = YEAR(NOW())";
                    break;
                default:
                    $where = "AND DATE(e.horodatage) = CURDATE()";
                    break;
            }

            $sql = "
            SELECT 
                DATE_FORMAT(e.horodatage, '%d-%m-%Y %H:%i') as horodatage,
                e.type_alerte,
                e.message_erreur
            FROM EVENEMENT e
            WHERE e.id_borne = :id_borne
            $where
            ORDER BY e.horodatage ASC
        ";

            $stmt = $this->pdo->prepare($sql);
            $params = [':id_borne' => $id_borne];

            if ($plage === 'semaine')
                $params[':semaine'] = $semaine;
            if ($plage === 'mois')
                $params[':mois'] = $mois;

            $stmt->execute($params);
            return $stmt->fetchAll(PDO::FETCH_ASSOC);
        } catch (PDOException $e) {
            error_log("Erreur GetEvenements : " . $e->getMessage());
            return [];
        }
    }*/

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
                SELECT m.horodatage, m.puissance as puissance
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
