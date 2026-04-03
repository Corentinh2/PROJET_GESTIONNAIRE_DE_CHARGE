<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Créer un compte</title>
        <style>
            body {
                background-color: #121212;
                color: white;
                font-family: sans-serif;
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                margin: 0;
            }
            .login-container {
                background-color: #1e1e1e;
                padding: 40px;
                border-radius: 8px;
                width: 350px;
                text-align: center;
            }
            input {
                width: 100%;
                padding: 12px;
                margin: 10px 0;
                border: none;
                border-radius: 4px;
                box-sizing: border-box;
            }
            button {
                width: 100%;
                background-color: #00d4ff;
                color: black;
                padding: 12px;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                font-weight: bold;
            }
            a {
                color: #00d4ff;
                font-size: 0.85em;
                display: block;
                margin-top: 15px;
            }
        </style>
    </head>
    <body>
        <div class="login-container">
            <h2>Créer un compte</h2>
            <?php if (isset($erreur)) echo "<p style='color:red'>$erreur</p>"; ?>
            <?php if (isset($succes)) echo "<p style='color:green'>$succes</p>"; ?>
            <form action="/index.php?action=CreerCompte" method="POST">
                <input type="text" name="identifiant" placeholder="Identifiant" value="<?= htmlspecialchars($identifiant ?? '') ?>" required>
                <input type="password" name="password" placeholder="Mot de passe" required>
                <input type="password" name="password_confirm" placeholder="Confirmer le mot de passe" required>
                <button type="submit">CRÉER LE COMPTE</button>
            </form>
            <a href="/index.php?action=Connexion">← Retour à la connexion</a>
        </div>
    </body>
</html>