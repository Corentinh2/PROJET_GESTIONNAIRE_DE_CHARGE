<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Connexion</title>
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
        </style>
    </head>
    <body>
        <div class="login-container">
            <h2>Connexion</h2>
            <?php if (isset($erreur)) echo "<p style='color:red'>$erreur</p>"; ?>
            <form action="/index.php?action=Connexion" method="POST">
                <input type="text" name="identifiant" placeholder="Identifiant" required>
                <input type="password" name="password" placeholder="Mot de passe" required>
                <button type="submit">SE CONNECTER</button>
            </form>

        </div>
    </body>
</html>