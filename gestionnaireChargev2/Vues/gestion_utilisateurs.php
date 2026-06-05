<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Gestion des utilisateurs</title>
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
            .table-section {
                background-color: #1a1a1a;
                border-radius: 10px;
                padding: 25px;
                border: 1px solid #2a2a2a;
            }
            .section-title {
                font-size: 1.1em;
                font-weight: bold;
                margin-bottom: 20px;
                color: white;
            }
            table {
                width: 100%;
                border-collapse: collapse;
            }
            th {
                text-align: left;
                padding: 12px 15px;
                color: #aaaaaa;
                font-weight: 600;
                border-bottom: 1px solid #2a2a2a;
                font-size: 0.9em;
            }
            td {
                padding: 12px 15px;
                border-bottom: 1px solid #1e1e1e;
                font-size: 0.9em;
            }
            tr:last-child td {
                border-bottom: none;
            }
            tr:hover td {
                background-color: #1e1e1e;
            }
            .badge {
                padding: 4px 10px;
                border-radius: 20px;
                font-size: 0.8em;
                font-weight: bold;
            }
            .badge-actif {
                background-color: #1a3a2a;
                color: #2ecc71;
                border: 1px solid #2ecc71;
            }
            .badge-suspendu {
                background-color: #3a1a1a;
                color: #e74c3c;
                border: 1px solid #e74c3c;
            }
            .btn {
                padding: 5px 12px;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                font-size: 0.8em;
                margin-right: 4px;
            }
            .btn-modifier {
                background-color: #f39c12;
                color: black;
            }
            .btn-supprimer {
                background-color: #e74c3c;
                color: white;
            }
            .btn-suspendre {
                background-color: #555;
                color: white;
            }
            .btn-reactiver {
                background-color: #2ecc71;
                color: black;
            }
            .role-admin {
                color: #f39c12;
            }
            .role-user {
                color: #00d4ff;
            }
        </style>
    </head>
    <body>
        <div class="sidebar">
            <div class="sidebar-title">BORNE</div>
            <a href="/index.php?action=ChoixParam" class="nav-item">Tableau de bord</a>
            <a href="/index.php?action=AjouterBorne" class="nav-item">Ajouter une borne</a>
            <a href="/index.php?action=GestionUtilisateurs" class="nav-item active">Gestion utilisateurs</a>
            <div class="sidebar-bottom">
                <a href="/index.php?action=Deconnexion" class="btn-sidebar btn-deconnexion">🔴 Déconnexion</a>
                <a href="/index.php?action=ChoixParam" class="btn-sidebar btn-retour">← Retour aux bornes</a>
            </div>
        </div>

        <div class="main-content">
            <div class="table-section">
                <div class="section-title">Gestion des utilisateurs</div>
                <table>
                    <thead>
                        <tr>
                            <th>Identifiant</th>
                            <th>Rôle</th>
                            <th>État</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php foreach ($listeUtilisateurs as $u): ?>
                            <tr>
                                <td><?= htmlspecialchars($u['login']) ?></td>
                                <td class="role-user">Utilisateur</td>
                                <td><span class="badge <?= $u['etat'] ? 'badge-actif' : 'badge-suspendu' ?>"><?= $u['etat'] ? 'Actif' : 'Suspendu' ?></span></td>
                                <td>
                                    <a href="/index.php?action=ModifierUtilisateur&id=<?= $u['id_utilisateur'] ?>">
                                        <button class="btn btn-modifier">Modifier</button>
                                    </a>
                                    <a href="/index.php?action=SupprimerUtilisateur&id=<?= $u['id_utilisateur'] ?>"
                                       onclick="return confirm('Supprimer cet utilisateur ?')">
                                        <button class="btn btn-supprimer">Supprimer</button>
                                    </a>
                                    <a href="/index.php?action=SuspendreUtilisateur&id=<?= $u['id_utilisateur'] ?>">
                                        <button class="btn btn-suspendre">Suspendre</button>
                                    </a>
                                    <a href="/index.php?action=ReactiverUtilisateur&id=<?= $u['id_utilisateur'] ?>">
                                        <button class="btn btn-reactiver">Réactiver</button>
                                    </a>
                                </td>
                            </tr>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            </div>
        </div>
    </body>
</html>