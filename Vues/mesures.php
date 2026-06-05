<?php
session_start();
if (!isset($_SESSION['connecte']) || $_SESSION['connecte'] !== true) {
    header("Location: login.php");
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Dernières mesures</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <link href="js/libs/datatables/css/dataTables.bootstrap5.min.css" rel="stylesheet"/>
        <script src="js/libs/datatables/js/jquery.dataTables.min.js"></script>
        <script src="js/libs/datatables/js/dataTables.bootstrap5.min.js"></script>
        <script src="mesures.js"></script>
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
            .page-header {
                margin-bottom: 28px;
            }
            .page-header h2 {
                font-family: 'Rajdhani', sans-serif;
                font-size: 1.8rem;
                font-weight: 700;
                color: #fff;
            }
            .page-header p {
                color: var(--muted);
                font-size: 0.85rem;
                margin-top: 4px;
            }

            .table-card {
                background: var(--glass);
                border: 1px solid var(--border);
                border-radius: 16px;
                backdrop-filter: blur(12px);
                overflow: hidden;
                box-shadow: 0 4px 32px rgba(0,0,0,0.3);
            }
            .table-card > div {
                padding: 20px;
            }

            #table_mesures {
                color: var(--text);
                border-color: var(--border) !important;
            }
            #table_mesures thead th {
                background: rgba(45,125,210,0.35) !important;
                color: #fff !important;
                font-family: 'Rajdhani', sans-serif;
                font-size: 0.8rem;
                font-weight: 700;
                letter-spacing: 0.1em;
                text-transform: uppercase;
                border-color: var(--border) !important;
                padding: 14px 12px;
            }
            #table_mesures tbody tr {
                background: rgba(10,22,40,0.6) !important;
                border-color: var(--border) !important;
                transition: background 0.15s;
                color: var(--text) !important;
            }
            #table_mesures tbody tr:hover td {
                background: rgba(45,125,210,0.07) !important;
            }
            #table_mesures tbody td {
                border-color: var(--border) !important;
                padding: 12px;
                font-size: 0.92rem;
            }
            #table_mesures.table-striped > tbody > tr:nth-of-type(odd) > td {
                background: rgba(255,255,255,0.04) !important;
                color: var(--text) !important;
            }

            /* Force all table cells dark */
            .table > :not(caption) > * > * {
                background-color: transparent !important;
                color: var(--text) !important;
            }
            .table-striped > tbody > tr:nth-of-type(odd) > * {
                background-color: rgba(255,255,255,0.04) !important;
                color: var(--text) !important;
            }
            .table-striped > tbody > tr:nth-of-type(even) > * {
                background-color: rgba(10,22,40,0.5) !important;
                color: var(--text) !important;
            }
            .dataTables_wrapper .dataTables_length label,
            .dataTables_wrapper .dataTables_filter label,
            .dataTables_wrapper .dataTables_info,
            .dataTables_wrapper .dataTables_paginate {
                color: var(--muted) !important;
                font-size: 0.83rem;
            }
            .dataTables_wrapper .dataTables_filter input,
            .dataTables_wrapper .dataTables_length select {
                background: rgba(255,255,255,0.05);
                border: 1px solid var(--border);
                border-radius: 8px;
                color: var(--text);
                padding: 5px 10px;
                outline: none;
            }
            .dataTables_wrapper .dataTables_filter input:focus {
                border-color: var(--accent);
            }
            .dataTables_wrapper .dataTables_paginate .paginate_button {
                color: var(--muted) !important;
                border-radius: 6px !important;
                border: none !important;
            }
            .dataTables_wrapper .dataTables_paginate .paginate_button.current,
            .dataTables_wrapper .dataTables_paginate .paginate_button:hover {
                background: rgba(45,125,210,0.2) !important;
                color: #fff !important;
                border: none !important;
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
                            <a class="nav-link" href="#" id="titreBorne"></a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="borne.php" id="navBorne">Tableau de bord</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link active" href="#" id="navMesures">30 dernières mesures</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="#" id="navDeconnexion">Déconnexion</a>
                        </li>
                    </ul>
                </div>
            </div>
        </nav>

        <div class="main-content">
            <div class="container-fluid">
                <div class="page-header">
                    <h2 id="titreMesures">Chargement...</h2>
                    <p>Historique des 30 dernières mesures enregistrées</p>
                </div>
                <div class="table-card">
                    <div>
                        <table id="table_mesures" class="table table-striped table-bordered text-center">
                            <thead>
                                <tr>
                                    <th class="text-center">id mesure</th>
                                    <th class="text-center">Horodatage</th>
                                    <th class="text-center">Puissance (kW)</th>
                                    <th class="text-center">id session</th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </body>
</html>