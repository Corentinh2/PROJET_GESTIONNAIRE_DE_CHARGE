<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Ajouter une borne</title>
        <style>
            * {
                margin: 0;
                padding: 0;
                box-sizing: border-box;
            }
            body {
                background-color: #0f0f0f;
                color: white;
                font-family: 'Segoe UI', sans-serif;
                display: flex;
                min-height: 100vh;
            }
            .sidebar {
                width: 260px;
                background-color: #1a1a1a;
                display: flex;
                flex-direction: column;
                padding: 20px;
                border-right: 1px solid #2a2a2a;
                position: fixed;
                height: 100vh;
            }
            .sidebar-title {
                color: #00d4ff;
                font-size: 1.1em;
                font-weight: bold;
                margin-bottom: 30px;
                padding-bottom: 15px;
                border-bottom: 1px solid #2a2a2a;
            }
            .nav-item {
                padding: 12px 15px;
                border-radius: 6px;
                cursor: pointer;
                margin-bottom: 5px;
                color: #aaaaaa;
                text-decoration: none;
                display: block;
                transition: 0.2s;
            }
            .nav-item:hover {
                background-color: #2a2a2a;
                color: white;
            }
            .nav-item.active {
                background-color: #1e3a4a;
                color: #00d4ff;
            }
            .sidebar-bottom {
                margin-top: auto;
                display: flex;
                flex-direction: column;
                gap: 10px;
            }
            .btn-sidebar {
                padding: 10px;
                border-radius: 6px;
                text-align: center;
                cursor: pointer;
                text-decoration: none;
                font-size: 0.9em;
                display: block;
            }
            .btn-deconnexion {
                background-color: #2a1a1a;
                color: #e74c3c;
                border: 1px solid #e74c3c;
            }
            .btn-retour {
                background-color: #1a2a2a;
                color: #00d4ff;
                border: 1px solid #00d4ff;
            }
            .main-content {
                margin-left: 260px;
                flex: 1;
                padding: 30px;
            }
            .form-section {
                background-color: #1a1a1a;
                border-radius: 10px;
                padding: 30px;
                border: 1px solid #2a2a2a;
                max-width: 600px;
            }
            .section-title {
                font-size: 1.1em;
                font-weight: bold;
                margin-bottom: 25px;
                color: white;
            }
            .form-group {
                margin-bottom: 20px;
            }
            .form-group label {
                display: block;
                color: #aaaaaa;
                font-size: 0.9em;
                margin-bottom: 8px;
            }
            .form-group input {
                width: 100%;
                padding: 12px 15px;
                background-color: #0f0f0f;
                border: 1px solid #2a2a2a;
                border-radius: 6px;
                color: white;
                font-size: 0.95em;
                transition: 0.2s;
            }
            .form-group input:focus {
                outline: none;
                border-color: #00d4ff;
            }
            .form-row {
                display: grid;
                grid-template-columns: 1fr 1fr;
                gap: 15px;
            }
            .btn-submit {
                width: 100%;
                padding: 12px;
                background-color: #00d4ff;
                color: #0f0f0f;
                border: none;
                border-radius: 6px;
                font-size: 1em;
                font-weight: bold;
                cursor: pointer;
                margin-top: 10px;
                transition: 0.2s;
            }
            .btn-submit:hover {
                background-color: #00b8d9;
            }
            .alert-succes {
                background-color: #1a3a2a;
                border: 1px solid #2ecc71;
                color: #2ecc71;
                padding: 12px 15px;
                border-radius: 6px;
                margin-bottom: 20px;
            }
            .alert-erreur {
                background-color: #3a1a1a;
                border: 1px solid #e74c3c;
                color: #e74c3c;
                padding: 12px 15px;
                border-radius: 6px;
                margin-bottom: 20px;
            }
        </style>
    </head>
    <body>
        <div class="sidebar">
            <div class="sidebar-title">BORNE</div>
            <a href="/index.php?action=ChoixParam" class="nav-item">Tableau de bord</a>
            <a href="/index.php?action=GestionUtilisateurs" class="nav-item">Gestion utilisateurs</a>
            <a href="/index.php?action=AjouterBorne" class="nav-item active">Ajouter une borne</a>
            <div class="sidebar-bottom">
                <a href="/index.php?action=Deconnexion" class="btn-sidebar btn-deconnexion">🔴 Déconnexion</a>
                <a href="/index.php?action=ChoixParam" class="btn-sidebar btn-retour">← Retour aux bornes</a>
            </div>
        </div>

        <div class="main-content">
            <div class="form-section">
                <div class="section-title">Ajouter une nouvelle borne</div>

                <?php if (isset($succes)): ?>
                    <div class="alert-succes">✅ <?= $succes ?></div>
                <?php endif; ?>
                <?php if (isset($erreur)): ?>
                    <div class="alert-erreur">❌ <?= $erreur ?></div>
                <?php endif; ?>

                <form action="/index.php?action=AjouterBorne" method="POST">
                    <div class="form-row">
                        <div class="form-group">
                            <label>Nom de la borne</label>
                            <input type="text" name="nom_borne" placeholder="Ex: Borne A1" required>
                        </div>
                        <div class="form-group">
                            <label>Emplacement</label>
                            <input type="text" name="emplacement" placeholder="Ex: Parking Nord" required>
                        </div>
                    </div>
                    <div class="form-row">
                        <div class="form-group">
                            <label>Puissance (W)</label>
                            <input type="number" name="puissance" placeholder="Ex: 7400" required>
                        </div>
                        <div class="form-group">
                            <label>Adresse IP usine</label>
                            <input type="text" name="ip_usine" placeholder="Ex: 192.168.1.10">
                        </div>
                    </div>
                    <button type="submit" class="btn-submit">AJOUTER LA BORNE</button>
                </form>
            </div>
        </div>
    </body>
</html>