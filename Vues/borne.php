<?php
session_start();
if (!isset($_SESSION['connecte']) || $_SESSION['connecte'] !== true) {
    header("Location: login.php");
    exit();
}
$isAdmin = isset($_SESSION['utilisateur']) && $_SESSION['utilisateur'] === 'admin';
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Détail de la borne</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <script src="https://code.highcharts.com/highcharts.js"></script>
        <script src="https://code.highcharts.com/modules/exporting.js"></script>
        <script src="https://code.highcharts.com/modules/export-data.js"></script>
        <script src="borne.js"></script>
        <link href="https://fonts.googleapis.com/css2?family=Rajdhani:wght@500;700&family=DM+Sans:wght@300;400;500&display=swap" rel="stylesheet">
        <style>
            :root {
                --navy:   #0a1628;
                --blue:   #1a3a6b;
                --accent: #2d7dd2;
                --bright: #4fa3e8;
                --glass:  rgba(255,255,255,0.04);
                --border: rgba(77,163,232,0.18);
                --text:   #e8eef8;
                --muted:  #7a9bbf;
            }
            *, *::before, *::after {
                box-sizing: border-box;
                margin: 0;
                padding: 0;
            }
            html, body {
                min-height: 100%;
                background: var(--navy);
                font-family: 'DM Sans', sans-serif;
                color: var(--text);
            }
            body::before {
                content: '';
                position: fixed;
                inset: 0;
                background-image:
                    linear-gradient(rgba(45,125,210,0.07) 1px, transparent 1px),
                    linear-gradient(90deg, rgba(45,125,210,0.07) 1px, transparent 1px);
                background-size: 40px 40px;
                animation: gridScroll 20s linear infinite;
                pointer-events: none;
                z-index: 0;
            }
            @keyframes gridScroll {
                0%   {
                    background-position: 0 0;
                }
                100% {
                    background-position: 40px 40px;
                }
            }
            .blob {
                position: fixed;
                border-radius: 50%;
                filter: blur(80px);
                opacity: 0.12;
                pointer-events: none;
                z-index: 0;
            }
            .blob-1 {
                width: 500px;
                height: 500px;
                background: var(--accent);
                top: -150px;
                right: -150px;
            }
            .blob-2 {
                width: 350px;
                height: 350px;
                background: var(--bright);
                bottom: -100px;
                left: -80px;
            }

            .navbar {
                position: relative;
                z-index: 100;
                background: rgba(10,22,40,0.85) !important;
                backdrop-filter: blur(12px);
                border-bottom: 1px solid var(--border);
                padding: 0 24px;
            }
            .navbar-brand {
                font-family: 'Rajdhani', sans-serif;
                font-size: 1.35rem;
                font-weight: 700;
                color: #fff !important;
                display: flex;
                align-items: center;
                gap: 10px;
            }
            .navbar-brand::before {
                content: '⚡';
                display: inline-flex;
                align-items: center;
                justify-content: center;
                width: 32px;
                height: 32px;
                background: linear-gradient(135deg, var(--accent), var(--bright));
                border-radius: 8px;
                font-size: 1rem;
            }
            .nav-link {
                font-family: 'DM Sans', sans-serif;
                font-size: 0.85rem;
                font-weight: 500;
                color: var(--muted) !important;
                letter-spacing: 0.04em;
                text-transform: uppercase;
                padding: 8px 14px !important;
                border-radius: 8px;
                transition: color 0.2s, background 0.2s;
            }
            .nav-link:hover, .nav-link.active {
                color: var(--text) !important;
                background: rgba(45,125,210,0.12);
            }
            .nav-link#navDeconnexion:hover {
                color: #e74a3b !important;
                background: rgba(231,74,59,0.1);
            }

            .main-content {
                position: relative;
                z-index: 10;
                padding: 40px 24px;
            }

            /* FILTRE DATE */
            .filter-card {
                background: var(--glass);
                border: 1px solid var(--border);
                border-radius: 14px;
                backdrop-filter: blur(12px);
                padding: 20px 24px;
                margin-bottom: 24px;
                display: flex;
                align-items: center;
                gap: 16px;
                flex-wrap: wrap;
            }
            .filter-card label {
                font-family: 'DM Sans', sans-serif;
                font-size: 0.8rem;
                font-weight: 500;
                letter-spacing: 0.07em;
                text-transform: uppercase;
                color: var(--muted);
                white-space: nowrap;
            }
            .filter-card input[type="date"] {
                background: rgba(255,255,255,0.05);
                border: 1px solid var(--border);
                border-radius: 10px;
                color: var(--text);
                padding: 10px 14px;
                font-family: 'DM Sans', sans-serif;
                font-size: 0.9rem;
                outline: none;
                transition: border-color 0.2s, box-shadow 0.2s;
                color-scheme: dark;
            }
            .filter-card input[type="date"]:focus {
                border-color: var(--accent);
                box-shadow: 0 0 0 3px rgba(45,125,210,0.18);
            }
            .filter-card .btn-afficher {
                padding: 10px 22px;
                border-radius: 10px;
                border: none;
                background: linear-gradient(135deg, var(--accent), var(--bright));
                color: #fff;
                font-family: 'Rajdhani', sans-serif;
                font-size: 0.95rem;
                font-weight: 700;
                letter-spacing: 0.07em;
                text-transform: uppercase;
                cursor: pointer;
                transition: opacity 0.2s, transform 0.15s;
                box-shadow: 0 4px 14px rgba(45,125,210,0.35);
            }
            .filter-card .btn-afficher:hover {
                opacity: 0.88;
                transform: translateY(-1px);
            }

            /* GRAPHE CARD */
            .chart-card {
                background: var(--glass);
                border: 1px solid var(--border);
                border-radius: 16px;
                backdrop-filter: blur(12px);
                padding: 24px;
                box-shadow: 0 4px 32px rgba(0,0,0,0.3);
            }
            #graphe {
                width: 100%;
                aspect-ratio: 2/1;
                border-radius: 10px;
                overflow: hidden;
            }
        </style>
    </head>
    <body>
        <div class="blob blob-1"></div>
        <div class="blob blob-2"></div>

        <nav class="navbar navbar-expand-lg">
            <div class="container-fluid">
                <a class="navbar-brand" href="#">GestioBorne</a>
                <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarAdmin">
                    <span class="navbar-toggler-icon"></span>
                </button>
                <div class="collapse navbar-collapse" id="navbarAdmin">
                    <ul class="navbar-nav me-auto mb-2 mb-lg-0">
                        <li class="nav-item">
                            <a class="nav-link" id="titreBorne"></a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link active" href="choix_bornes.php" id="navBorne">Choix borne</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="mesures.php" id="navMariaDb">Historique Mariadb</a>
                        </li>
                        <?php if ($isAdmin): ?>
                            <li class="nav-item">
                                <a class="nav-link" href="admin.php" id="navAdmin">Administration</a>
                            </li>
                        <?php endif; ?>
                        <li class="nav-item">
                            <a class="nav-link" href="#" id="navDeconnexion">Déconnexion</a>
                        </li>
                    </ul>
                </div>
            </div>
        </nav>

        <div class="main-content">
            <div class="container-fluid">
                <div class="filter-card">
                    <label for="dateDebut">Choisissez une durée&nbsp;:</label>
                    <input type="date" id="dateDebut"/>
                    <input type="date" id="datefin"/>
                    <input type="button" class="btn-afficher" id="btnAfficher" value="Afficher"/>
                    <input type="hidden" id="idBorne"/>
                </div>
                <div class="chart-card">
                    <div id="graphe"></div>
                </div>
            </div>
        </div>

    </body>
</html>