<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Sélection de la borne</title>
        <style>
            body {
                background-color: #121212;
                color: white;
                font-family: 'Segoe UI', sans-serif;
                text-align: center;
                padding: 40px;
            }
            h1 {
                color: #00d4ff;
            }
            .container {
                display: grid;
                grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
                gap: 20px;
                max-width: 1200px;
                margin: 0 auto;
            }
            .borne-card {
                background-color: #1e1e1e;
                border-radius: 8px;
                padding: 20px;
                text-align: left;
                transition: 0.2s;
                cursor: pointer;
                border: 1px solid transparent;
            }
            .borne-card:hover {
                transform: translateY(-5px);
                border-color: #00d4ff;
            }
            .power {
                color: #00d4ff;
                font-weight: bold;
                font-size: 1.2em;
                text-align: right;
            }
            .nav {
                margin-bottom: 20px;
                text-align: right;
            }
            .nav a {
                color: #00d4ff;
                text-decoration: none;
                border: 1px solid #00d4ff;
                padding: 8px 16px;
                border-radius: 4px;
            }
        </style>
    </head>
    <body>
        <div class="nav">
            <a href="/index.php?action=Deconnexion">🔴 Déconnexion</a>
        </div>
        <h1>SÉLECTION DE LA BORNE</h1>
        <div class="container">
            <?php foreach ($listeBornes as $borne): ?>
                <div class="borne-card" onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $borne['id_borne'] ?>'">
                    <strong>Borne #<?= $borne['id_borne'] ?> - <?= htmlspecialchars($borne['nom_borne']) ?></strong>
                    <p><?= htmlspecialchars($borne['emplacement']) ?></p>
                    <div class="power"><?= $borne['puissance'] ?> W</div>
                </div>
            <?php endforeach; ?>
        </div>
    </body>
</html>