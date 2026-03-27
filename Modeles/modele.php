<?php
class Modele {
    private $db;

    public function __construct() {
        try {
            // Paramètres à adapter selon votre configuration NetBeans/WAMP/XAMPP
            $host = 'localhost';
            $dbname = 'votre_bdd';
            $user = 'root';
            $pass = '';
            
            $this->db = new PDO("mysql:host=$host;dbname=$dbname;charset=utf8", $user, $pass);
            $this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        } catch (PDOException $e) {
            die("Erreur de connexion : " . $e->getMessage());
        }
    }

    public function LoginVerify($login) {
        $stmt = $this->db->prepare("SELECT * FROM utilisateurs WHERE identifiant = :login LIMIT 1");
        $stmt->execute(['login' => $login]);
        return $stmt->fetch(PDO::FETCH_ASSOC);
    }

    public function PasswordVerify($password, $hash) {
        // Utilise la vérification native PHP (compatible avec password_hash)
        return password_verify($password, $hash);
    }

    public function GetDonnees() {
        // Simule la récupération des valeurs pour le graphique
        return [12, 19, 3, 5, 2, 3];
    }
/*Wake up neéo!!!
    
    // Autres méthodes de votre diagramme
    public function RecupererListeBorne() { }
    public function RecupererDetailsBorn() { }
    public function RecupererConfig() { }
    public function UpdateConfig() { }
    public function GetMesures() { }
    public function EstimationVolume() { }
    public function RecupererValeurs() { }
*/
 }
 