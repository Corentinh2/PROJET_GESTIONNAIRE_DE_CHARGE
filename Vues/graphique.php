<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Tableau de bord - Borne #<?= htmlspecialchars($_GET['id'] ?? '') ?></title>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/highcharts.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/exporting.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/export-data.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/accessibility.js"></script>
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

            /* SIDEBAR */
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

            /* CONTENU */
            .main-content {
                margin-left: 260px;
                flex: 1;
                padding: 30px;
            }
            .page-title {
                font-size: 1.4em;
                font-weight: bold;
                margin-bottom: 25px;
                color: white;
            }

            /* CARTES */
            .cards-row {
                display: grid;
                grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
                gap: 20px;
                margin-bottom: 25px;
            }
            .card {
                background-color: #1a1a1a;
                border-radius: 10px;
                padding: 25px;
                border: 1px solid #2a2a2a;
            }
            .card-label {
                font-size: 0.9em;
                color: #aaaaaa;
                margin-bottom: 10px;
            }
            .card-value {
                font-size: 2em;
                font-weight: bold;
                color: #00d4ff;
            }
            .card-sub {
                font-size: 0.8em;
                color: #666;
                margin-top: 5px;
            }
            .card-status {
                font-size: 1.1em;
                font-weight: bold;
                color: #2ecc71;
            }
            .card-status-sub {
                font-size: 0.85em;
                color: #2ecc71;
                margin-top: 5px;
            }

            /* GRAPHIQUE */
            .chart-section {
                background-color: #1a1a1a;
                border-radius: 10px;
                padding: 25px;
                border: 1px solid #2a2a2a;
                margin-bottom: 25px;
            }
            .section-title {
                font-size: 1.1em;
                font-weight: bold;
                margin-bottom: 20px;
                color: white;
            }
            .plage-controls {
                display: flex;
                gap: 10px;
                margin-bottom: 20px;
            }
            .btn-plage {
                padding: 8px 20px;
                border-radius: 5px;
                border: 1px solid #00d4ff;
                background: transparent;
                color: #00d4ff;
                cursor: pointer;
                font-size: 0.9em;
                transition: 0.2s;
            }
            .btn-plage.active {
                background-color: #00d4ff;
                color: #0f0f0f;
                font-weight: bold;
            }

            /* TABLEAU */
            .table-section {
                background-color: #1a1a1a;
                border-radius: 10px;
                padding: 25px;
                border: 1px solid #2a2a2a;
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

            .selecteur {
                margin-bottom: 15px;
                display: flex;
                align-items: center;
                gap: 10px;
            }
            .selecteur label {
                color: #aaaaaa;
                font-size: 0.9em;
            }
            .selecteur select {
                background-color: #0f0f0f;
                border: 1px solid #2a2a2a;
                color: white;
                padding: 8px 12px;
                border-radius: 6px;
                cursor: pointer;
            }
            .selecteur select:focus {
                outline: none;
                border-color: #00d4ff;
            }
        </style>
    </head>
    <body>
        <!-- SIDEBAR -->
        <div class="sidebar">
            <div class="sidebar-title">Borne #<?= str_pad($_GET['id'] ?? '', 2, '0', STR_PAD_LEFT) ?> - <?= htmlspecialchars($nomBorne ?? '') ?></div>
            <a href="/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=<?= $_GET['plage'] ?? 'jour' ?>" class="nav-item active">Tableau de bord</a>
            <a href="/index.php?action=Historique&id=<?= $_GET['id'] ?>" class="nav-item">Historique MariaDB</a>
            <?php if (isset($_SESSION['role']) && $_SESSION['role'] === 'admin'): ?>
                <a href="/index.php?action=GestionUtilisateurs" class="nav-item">Gestion utilisateurs</a>
            <?php endif; ?>
            <div class="sidebar-bottom">
                <a href="/index.php?action=Deconnexion" class="btn-sidebar btn-deconnexion">🔴 Déconnexion</a>
                <a href="/index.php?action=ChoixParam" class="btn-sidebar btn-retour">← Retour aux bornes</a>
            </div>
        </div>

        <!-- CONTENU -->
        <div class="main-content">
            <div class="cards-row">
                <div class="card">
                    <div class="card-label">Puissance Actuelle</div>
                    <div class="card-value">
                        <?=
                        ($derniereMesure && isset($derniereMesure['puissance'])) ? number_format((float) $derniereMesure['puissance'], 2, ',', ' ') . ' kW' : 'Aucune donnée'
                        ?>
                    </div>
                    <div class="card-sub">
                        Dernière mise à jour : <?=
                        ($derniereMesure && isset($derniereMesure['horodatage'])) ? $derniereMesure['horodatage'] : '-'
                        ?>
                    </div>
                </div>
                <div class="card">
                    <div class="card-label">État Relais</div>
                    <div class="card-status">ACTIF (ON)</div>
                    <div class="card-status-sub">État borne : Active</div>
                </div>
            </div>

            <div class="chart-section">
                <div class="section-title">Consommation de Puissance (KW) - Historique Récent</div>
                <div class="plage-controls">
                    <button class="btn-plage <?= (!isset($_GET['plage']) || $_GET['plage'] === 'jour') ? 'active' : '' ?>"
                            onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=jour'">Jour</button>
                    <button class="btn-plage <?= (isset($_GET['plage']) && $_GET['plage'] === 'semaine') ? 'active' : '' ?>"
                            onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=semaine'">Semaine</button>
                    <button class="btn-plage <?= (isset($_GET['plage']) && $_GET['plage'] === 'mois') ? 'active' : '' ?>"
                            onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=mois'">Mois</button>
                </div>

                <!-- Sélecteur dynamique selon la plage -->
                <?php if (isset($_GET['plage']) && $_GET['plage'] === 'semaine'): ?>
                    <div class="selecteur">
                        <label>Choisir la semaine :</label>
                        <select onchange="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=semaine&semaine=' + this.value">
                            <?php for ($i = 1; $i <= 52; $i++): ?>
                                <option value="<?= $i ?>" <?= (isset($_GET['semaine']) && $_GET['semaine'] == $i) ? 'selected' : '' ?>>
                                    Semaine <?= $i ?>
                                </option>
                            <?php endfor; ?>
                        </select>
                    </div>
                <?php endif; ?>

                <?php if (isset($_GET['plage']) && $_GET['plage'] === 'mois'): ?>
                    <div class="selecteur">
                        <label>Choisir le mois :</label>
                        <select onchange="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=mois&mois=' + this.value">
                            <?php
                            $mois = ['Janvier', 'Février', 'Mars', 'Avril', 'Mai', 'Juin', 'Juillet', 'Août', 'Septembre', 'Octobre', 'Novembre', 'Décembre'];
                            foreach ($mois as $num => $nom):
                                ?>
                                <option value="<?= $num + 1 ?>" <?= (isset($_GET['mois']) && $_GET['mois'] == $num + 1) ? 'selected' : '' ?>>
                                    <?= $nom ?>
                                </option>
                            <?php endforeach; ?>
                        </select>
                    </div>
                <?php endif; ?>
                <div id="chart-container"></div>
            </div>
        </div>

        
        <script>
            const categories = <?php echo json_encode(array_column($mesures, 'horodatage')); ?>;   
            const data = <?php echo json_encode(array_map(fn($m) => round((float) $m['puissance'], 2), $mesures)); ?>;

            Highcharts.chart('chart-container', {
                chart: {type: 'column', backgroundColor: '#1a1a1a'},
                title: {text: 'Puissance mesuré (kW) - par tranche de 30 min'},
                xAxis: {
                    categories: categories,
                    labels: {style: {color: '#aaaaaa'}, rotation: -45},
                    lineColor: '#333', tickColor: '#333'
                },
                yAxis: {
                    title: {text: 'Puissance (kW)', style: {color: '#aaaaaa'}},
                    labels: {style: {color: '#aaaaaa'}},
                    gridLineColor: '#2a2a2a'
                },
                series: [
                    {
                        name: 'Puissance (kW)',
                        data: data,
                        color: '#00d4ff'
                    }], 
                        legend: {itemStyle: {color: '#aaaaaa'}},
                exporting: {
                    enabled: true,
                    buttons: {contextButton: {menuItems: ['downloadPNG', 'downloadCSV', 'downloadXLS']}}
                },
                credits: {enabled: false}
            });
        </script>
    </body>
</html>