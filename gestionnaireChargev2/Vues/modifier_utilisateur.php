<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Modifier l'utilisateur</title>
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
            .container {
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
                background-color: #f39c12;
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
        <div class="container">
            <h2>Modifier l'utilisateur</h2>
            <form action="/index.php?action=ModifierUtilisateur" method="POST">
                <input type="hidden" name="id" value="<?= $utilisateur['id_utilisateur'] ?>">
                <input type="text" name="login" value="<?= htmlspecialchars($utilisateur['login']) ?>" required>
                <input type="password" name="mdp" placeholder="Nouveau mot de passe" required>
                <button type="submit">ENREGISTRER</button>
            </form>
            <a href="/index.php?action=GestionUtilisateurs">← Retour</a>
        </div>
    </body>
</html>