<?php

class Modele {

    private $pdo;

    public function __construct() {
        try {
            $this->pdo = new PDO(
                    'mysql:host=localhost;dbname=gestionnaire_de_charge;charset=utf8',
                    'charge',
                    'ciel'
            );
            $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        } catch (PDOException $e) {
            die("Erreur de connexion : " . $e->getMessage());
        }
    }

    public function LoginVerify($login) {
        $stmt = $this->pdo->prepare("SELECT * FROM UTILISATEUR WHERE login = :login");
        $stmt->execute([':login' => $login]);
        $user = $stmt->fetch(PDO::FETCH_ASSOC);

        // On vérifie le mot de passe chiffré
        if ($user && password_verify($_POST['password'], $user['mdp'])) {
            return $user;
        }
        return false;
    }

    public function UtilisateurExiste($login) {
        $stmt = $this->pdo->prepare("SELECT COUNT(*) FROM UTILISATEUR WHERE login = :login");
        $stmt->execute([':login' => $login]);
        return $stmt->fetchColumn() > 0;
    }

    public function CreerUtilisateur($login, $mdp) {
        // On chiffre le mot de passe avant de l'enregistrer
        $mdp_chiffre = password_hash($mdp, PASSWORD_DEFAULT);
        $stmt = $this->pdo->prepare("INSERT INTO UTILISATEUR (login, mdp, date_creation, etat) VALUES (:login, :mdp, CURDATE(), 1)");
        $stmt->execute([':login' => $login, ':mdp' => $mdp_chiffre]);
    }

    public function GetMesuresParBorne($id_borne, $plage = 'jour') {
        switch ($plage) {
            case 'semaine':
                $interval = 'INTERVAL 7 DAY';
                break;
            case 'mois':
                $interval = 'INTERVAL 1 MONTH';
                break;
            default: // jour
                $interval = 'INTERVAL 1 DAY';
                break;
        }

        $sql = "
        SELECT 
            DATE_FORMAT(m.horodatage, '%Y-%m-%d %H:%i') as horodatage,
            AVG(m.puissance) as puissance
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
    }

    public function RecupererListeBorne() {
        $stmt = $this->pdo->query("SELECT id_borne, nom_borne, emplacement, puissance FROM BORNE");
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    public function RecupererUtilisateurs() {
        $stmt = $this->pdo->query("SELECT * FROM UTILISATEUR");
        return $stmt->fetchAll(PDO::FETCH_ASSOC);
    }

    public function RecupererUtilisateurParId($id) {
        $stmt = $this->pdo->prepare("SELECT * FROM UTILISATEUR WHERE id_utilisateur = :id");
        $stmt->execute([':id' => $id]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    public function SupprimerUtilisateur($id) {
        // 1. Récupérer les id_conducteur liés à cet utilisateur
        $stmt = $this->pdo->prepare("SELECT id_conducteur FROM CONDUCTEUR WHERE id_utilisateur = :id");
        $stmt->execute([':id' => $id]);
        $conducteurs = $stmt->fetchAll(PDO::FETCH_COLUMN);

        foreach ($conducteurs as $id_conducteur) {
            // 2. Récupérer les id_vehicule liés à ce conducteur
            $stmt = $this->pdo->prepare("SELECT id_vehicule FROM VEHICULE WHERE id_conducteur = :id_conducteur");
            $stmt->execute([':id_conducteur' => $id_conducteur]);
            $vehicules = $stmt->fetchAll(PDO::FETCH_COLUMN);

            foreach ($vehicules as $id_vehicule) {
                // 3. Récupérer les id_charge liés à ce véhicule
                $stmt = $this->pdo->prepare("SELECT id_charge FROM SESSION WHERE id_vehicule = :id_vehicule");
                $stmt->execute([':id_vehicule' => $id_vehicule]);
                $sessions = $stmt->fetchAll(PDO::FETCH_COLUMN);

                // 4. Supprimer les MESURE liées à ces sessions
                foreach ($sessions as $id_charge) {
                    $stmt = $this->pdo->prepare("DELETE FROM MESURE WHERE id_charge = :id_charge");
                    $stmt->execute([':id_charge' => $id_charge]);
                }

                // 5. Supprimer les SESSION liées à ce véhicule
                $stmt = $this->pdo->prepare("DELETE FROM SESSION WHERE id_vehicule = :id_vehicule");
                $stmt->execute([':id_vehicule' => $id_vehicule]);
            }

            // 6. Supprimer les VEHICULE liés à ce conducteur
            $stmt = $this->pdo->prepare("DELETE FROM VEHICULE WHERE id_conducteur = :id_conducteur");
            $stmt->execute([':id_conducteur' => $id_conducteur]);
        }

        // 7. Supprimer les CONDUCTEUR liés à l'utilisateur
        $stmt = $this->pdo->prepare("DELETE FROM CONDUCTEUR WHERE id_utilisateur = :id");
        $stmt->execute([':id' => $id]);

        // 8. Supprimer l'UTILISATEUR
        $stmt = $this->pdo->prepare("DELETE FROM UTILISATEUR WHERE id_utilisateur = :id");
        $stmt->execute([':id' => $id]);
    }

    public function ChangerEtatUtilisateur($id, $etat) {
        $stmt = $this->pdo->prepare("UPDATE UTILISATEUR SET etat = :etat WHERE id_utilisateur = :id");
        $stmt->execute([':etat' => $etat, ':id' => $id]);
    }

    public function ModifierUtilisateur($id, $login, $mdp) {
        // On chiffre aussi lors de la modification
        $mdp_chiffre = password_hash($mdp, PASSWORD_DEFAULT);
        $stmt = $this->pdo->prepare("UPDATE UTILISATEUR SET login = :login, mdp = :mdp WHERE id_utilisateur = :id");
        $stmt->execute([':login' => $login, ':mdp' => $mdp_chiffre, ':id' => $id]);
    }
}
