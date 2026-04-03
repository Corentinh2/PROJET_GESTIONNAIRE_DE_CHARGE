<!DOCTYPE html>
<html lang="fr">
    <head>
        <meta charset="UTF-8">
        <title>Graphique de Consommation</title>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/highcharts.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/exporting.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/export-data.js"></script>
        <script src="https://cdn.jsdelivr.net/npm/highcharts@11/modules/accessibility.js"></script>
        <style>
            body {
                background-color: #121212;
                color: white;
                font-family: sans-serif;
                display: flex;
                flex-direction: column;
                align-items: center;
                padding: 40px;
            }
            h1 {
                color: #00d4ff;
            }
            .controls {
                margin: 20px 0;
                display: flex;
                gap: 10px;
            }
            .btn-plage {
                padding: 10px 20px;
                border: 1px solid #00d4ff;
                background: transparent;
                color: #00d4ff;
                border-radius: 4px;
                cursor: pointer;
                font-size: 1em;
            }
            .btn-plage.active {
                background-color: #00d4ff;
                color: black;
                font-weight: bold;
            }
            #chart-container {
                width: 90%;
                max-width: 1000px;
            }
            .nav-btn {
                margin-top: 20px;
                color: #b0b0b0;
                text-decoration: none;
                border: 1px solid #333;
                padding: 10px 20px;
                border-radius: 5px;
            }
        </style>
    </head>
    <body>
        <h1>Consommation Borne #<?= htmlspecialchars($_GET['id'] ?? '') ?></h1>

        <div class="controls">
            <button class="btn-plage <?= (!isset($_GET['plage']) || $_GET['plage'] === 'jour') ? 'active' : '' ?>"
                    onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=jour'">
                Jour
            </button>
            <button class="btn-plage <?= (isset($_GET['plage']) && $_GET['plage'] === 'semaine') ? 'active' : '' ?>"
                    onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=semaine'">
                Semaine
            </button>
            <button class="btn-plage <?= (isset($_GET['plage']) && $_GET['plage'] === 'mois') ? 'active' : '' ?>"
                    onclick="window.location.href = '/index.php?action=EnregistreGraph&id=<?= $_GET['id'] ?>&plage=mois'">
                Mois
            </button>
        </div>

        <div id="chart-container"></div>
        <a href="/index.php?action=ChoixParam" class="nav-btn">⬅ Retour aux bornes</a>

        <script>
            const categories = <?php echo json_encode(array_column($mesures, 'horodatage')); ?>;
            const data = <?php echo json_encode(array_map(fn($m) => round((float) $m['puissance'], 2), $mesures)); ?>;

            Highcharts.chart('chart-container', {
                chart: {
                    type: 'column',
                    backgroundColor: '#1e1e1e',
                    style: {fontFamily: 'sans-serif'}
                },
                title: {
                    text: 'Puissance mesurée (W) — par tranche de 30 min',
                    style: {color: '#ffffff'}
                },
                xAxis: {
                    categories: categories,
                    labels: {style: {color: '#aaaaaa'}, rotation: -45},
                    lineColor: '#444',
                    tickColor: '#444'
                },
                yAxis: {
                    title: {text: 'Puissance (W)', style: {color: '#aaaaaa'}},
                    labels: {style: {color: '#aaaaaa'}},
                    gridLineColor: '#333'
                },
                series: [{
                        name: 'Puissance (W)',
                        data: data,
                        color: '#00d4ff'
                    }],
                legend: {itemStyle: {color: '#ffffff'}},
                exporting: {
                    enabled: true,
                    buttons: {
                        contextButton: {
                            menuItems: ['downloadPNG', 'downloadCSV', 'downloadXLS']
                        }
                    }
                },
                credits: {enabled: false}
            });
        </script>
    </body>
</html>