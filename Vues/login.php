<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Connexion</title>
    <style>
        body { background-color: #121212; color: white; font-family: 'Segoe UI', sans-serif; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
        .login-container { background-color: #1e1e1e; padding: 40px; border-radius: 8px; box-shadow: 0 4px 15px rgba(0, 0, 0, 0.5); width: 100%; max-width: 400px; text-align: center; }
        h2 { margin-bottom: 25px; font-weight: bold; }
        .error { color: #ff5555; background: rgba(255, 85, 85, 0.1); padding: 10px; border-radius: 4px; margin-bottom: 15px; }
        input { width: 100%; padding: 12px; margin: 8px 0; border: none; border-radius: 4px; box-sizing: border-box; background: #fff; }
        button { width: 100%; background-color: #00d4ff; color: black; padding: 12px; margin: 20px 0; border: none; border-radius: 4px; cursor: pointer; font-weight: bold; text-transform: uppercase; }
        button:hover { background-color: #00b8e6; }
        .test-accounts { font-size: 0.85em; color: #b0b0b0; line-height: 1.6; }
        .test-accounts b { color: #ffffff; }
    </style>
</head>
<body>
    <div class="login-container">
        <h2>Connexion</h2>
        <?php if(isset($erreur)): ?>
            <div class="error"><?php echo $erreur; ?></div>
        <?php endif; ?>
        <form action="index.php?action=Connexion" method="POST">
            <input type="text" name="identifiant" placeholder="Identifiant" required>
            <input type="password" name="password" placeholder="Mot de passe" required>
            <button type="submit">Se connecter</button>
        </form>
        <div class="test-accounts">
            Comptes de test : <b>toto/1234</b> (accès bornes), <b>toto/titi</b> (gestion utilisateurs)
        </div>
    </div>
</body>
</html>