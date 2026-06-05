<?php

class ModeleUtilisateur {

    private $pdo;

    

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
      } */

    

    

    
    

    public function getUtilisateurParLogin($login) {
        $stmt = $this->pdo->prepare(
                "SELECT * FROM UTILISATEUR WHERE login = :login AND etat = 1 LIMIT 1"
        );
        $stmt->execute([':login' => $login]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    

   /* public function GetConsommationTotale($id_borne, $plage = 'jour', $date_debut = null, $date_fin = null, $mois = null) {
        try {
            switch ($plage) {
                case 'semaine':
                    $sql = "
                    SELECT SUM(m.puissance)  as total
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND DATE(m.horodatage) BETWEEN :date_debut AND :date_fin
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([
                        ':id_borne' => $id_borne,
                        ':date_debut' => $date_debut,
                        ':date_fin' => $date_fin
                    ]);
                    break;

                case 'mois':
                    $sql = "
                    SELECT SUM(m.puissance)  as total
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND MONTH(m.horodatage) = :mois
                    AND YEAR(m.horodatage) = YEAR(NOW())
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne, ':mois' => $mois]);
                    break;

                default: // jour
                    $sql = "
                    SELECT SUM(m.puissance)  as total
                    FROM MESURE m
                    JOIN SESSION s ON m.id_charge = s.id_charge
                    WHERE s.id_borne = :id_borne
                    AND DATE(m.horodatage) = CURDATE()
                ";
                    $stmt = $this->pdo->prepare($sql);
                    $stmt->execute([':id_borne' => $id_borne]);
                    break;
            }
            $result = $stmt->fetch(PDO::FETCH_ASSOC);
            return $result['total'] ?? 0;
        } catch (PDOException $e) {
            error_log("Erreur GetConsommationTotale : " . $e->getMessage());
            return 0;
        }
    }*/
}
