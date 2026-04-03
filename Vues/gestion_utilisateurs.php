<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Gestion des utilisateurs</title>
        <style>
            body {
                background-color: #121212;
                color: white;
                font-family: sans-serif;
                padding: 40px;
            }
            h1 {
                color: #00d4ff;
            }
            table {
                width: 100%;
                border-collapse: collapse;
                margin-top: 20px;
                background: #1e1e1e;
                border-radius: 8px;
                overflow: hidden;
            }
            th {
                background-color: #00d4ff;
                color: black;
                padding: 12px;
                text-align: left;
            }
            td {
                padding: 12px;
                border-bottom: 1px solid #333;
            }
            .btn {
                padding: 6px 12px;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                font-size: 0.85em;
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
                background-color: #95a5a6;
                color: black;
            }
            .btn-reactiver {
                background-color: #2ecc71;
                color: black;
            }
            .etat-actif {
                color: #2ecc71;
                font-weight: bold;
            }
            .etat-suspendu {
                color: #e74c3c;
                font-weight: bold;
            }
            .nav {
                margin-bottom: 20px;
                display: flex;
                gap: 10px;
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
            <a href="/index.php?action=ChoixParam">← Retour aux bornes</a>
            <a href="/index.php?action=Deconnexion">🔴 Déconnexion</a>
        </div>
        <h1>Gestion des utilisateurs</h1>
        <table>
            <thead>
                <tr>
                    <th>ID</th>
                    <th>Identifiant</th>
                    <th>Date création</th>
                    <th>État</th>
                    <th>Actions</th>
                </tr>
            </thead>
            <tbody>
                <?php foreach ($listeUtilisateurs as $u): ?>
                    <tr>
                        <td><?= $u['id_utilisateur'] ?></td>
                        <td><?= htmlspecialchars($u['login']) ?></td>
                        <td><?= $u['date_creation'] ?></td>
                        <td class="<?= $u['etat'] ? 'etat-actif' : 'etat-suspendu' ?>">
                            <?= $u['etat'] ? 'Actif' : 'Suspendu' ?>
                        </td>
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
    </body>
</html>