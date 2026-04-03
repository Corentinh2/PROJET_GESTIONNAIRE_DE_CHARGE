<?php

class VueClient {

    public function DessinerGraphique($donnees) {
        ?>
        <!DOCTYPE html>
        <html lang="fr">
            <head>
                <meta charset="UTF-8">
                <title>Graphique de Consommation</title>
                <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
                <style>
                    body {
                        background-color: #121212;
                        color: white;
                        font-family: sans-serif;
                        display: flex;
                        flex-direction: column;
                        align-items: center;
                        padding-top: 50px;
                    }
                    .chart-box {
                        background: #1e1e1e;
                        padding: 30px;
                        border-radius: 10px;
                        width: 80%;
                        max-width: 800px;
                        box-shadow: 0 4px 15px rgba(0,0,0,0.5);
                    }
                    h2 {
                        color: #00d4ff;
                        text-transform: uppercase;
                    }
                    .nav-btn {
                        margin-top: 20px;
                        color: #b0b0b0;
                        text-decoration: none;
                        border: 1px solid #333;
                        padding: 10px;
                        border-radius: 5px;
                    }
                </style>
            </head>
            <body>
                <div class="chart-box">
                    <h2>Analyse de la Borne</h2>
                    <canvas id="myChart"></canvas>
                </div>
                <a href="index.php?action=ChoixParam" class="nav-btn">⬅ Retour aux bornes</a>

                <script>
                    const ctx = document.getElementById('myChart').getContext('2d');
                    new Chart(ctx, {
                        type: 'line',
                        data: {
                            labels: ['00h', '04h', '08h', '12h', '16h', '20h'],
                            datasets: [{
                                    label: 'Puissance Mesurée (W)',
                                    data: <?php echo json_encode($donnees); ?>,
                                    borderColor: '#00d4ff',
                                    backgroundColor: 'rgba(0, 212, 255, 0.1)',
                                    fill: true,
                                    tension: 0.4
                                }]
                        },
                        options: {
                            responsive: true,
                            scales: {
                                y: {beginAtZero: true, grid: {color: '#333'}},
                                x: {grid: {color: '#333'}}
                            }
                        }
                    });
                </script>
            </body>
        </html>
        <?php
    }
}
