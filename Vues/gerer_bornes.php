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
        <title>Gestion des bornes</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet"/>
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="js/libs/bootstrap/js/bootstrap.bundle.min.js"></script>
        <link href="js/libs/datatables/css/dataTables.bootstrap5.min.css" rel="stylesheet"/>
        <script src="js/libs/datatables/js/jquery.dataTables.min.js"></script>
        <script src="js/libs/datatables/js/dataTables.bootstrap5.min.js"></script>
        <script src="gerer_bornes.js"></script>
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
                display: flex;
                align-items: flex-end;
                justify-content: space-between;
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

            #table_borne {
                color: var(--text);
                border-color: var(--border) !important;
            }
            #table_borne thead th {
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
            #table_borne tbody tr {
                background: rgba(10,22,40,0.6) !important;
                border-color: var(--border) !important;
                transition: background 0.15s;
                color: var(--text) !important;
            }
            #table_borne tbody tr:hover td {
                background: rgba(45,125,210,0.07) !important;
            }
            #table_borne tbody td {
                border-color: var(--border) !important;
                padding: 12px;
                font-size: 0.92rem;
            }
            #table_borne.table-striped > tbody > tr:nth-of-type(odd) > td {
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

            /* Bouton ajouter dans le header du tableau */
            #ajouterBorne {
                display: inline-flex;
                align-items: center;
                justify-content: center;
                width: 22px;
                height: 22px;
                background: linear-gradient(135deg, var(--accent), var(--bright));
                border-radius: 6px;
                color: #fff !important;
                font-size: 1.1rem;
                line-height: 1;
                transition: opacity 0.2s, transform 0.15s;
                box-shadow: 0 2px 8px rgba(45,125,210,0.4);
            }
            #ajouterBorne:hover {
                opacity: 0.85;
                transform: scale(1.1);
            }

            .mod  {
                cursor: pointer;
                color: var(--bright);
                transition: opacity 0.2s;
            }
            .supp {
                cursor: pointer;
                color: #e74a3b;
                transition: opacity 0.2s;
            }
            .mod:hover, .supp:hover {
                opacity: 0.7;
            }

            /* MODALS */
            .modal-content {
                background: #0f1e36;
                border: 1px solid var(--border);
                border-radius: 16px;
                color: var(--text);
            }
            .modal-header {
                border-bottom: 1px solid var(--border);
                padding: 20px 24px;
            }
            .modal-header .modal-title {
                font-family: 'Rajdhani', sans-serif;
                font-size: 1.2rem;
                font-weight: 700;
                color: #fff;
            }
            .modal-header .btn-close {
                filter: invert(1) opacity(0.6);
            }
            .modal-body {
                padding: 24px;
            }
            .modal-footer {
                border-top: 1px solid var(--border);
                padding: 16px 24px;
            }
            .form-label {
                font-size: 0.75rem;
                font-weight: 500;
                letter-spacing: 0.08em;
                text-transform: uppercase;
                color: var(--muted);
                margin-bottom: 7px;
            }
            .form-control {
                background: rgba(255,255,255,0.05);
                border: 1px solid var(--border);
                border-radius: 10px;
                color: var(--text);
                padding: 11px 14px;
                font-family: 'DM Sans', sans-serif;
                font-size: 0.93rem;
                transition: border-color 0.2s, box-shadow 0.2s;
            }
            .form-control:focus {
                background: rgba(255,255,255,0.07);
                border-color: var(--accent);
                box-shadow: 0 0 0 3px rgba(45,125,210,0.18);
                color: var(--text);
                outline: none;
            }
            .btn-secondary {
                background: transparent;
                border: 1px solid var(--border);
                color: var(--muted);
                border-radius: 8px;
                font-family: 'DM Sans', sans-serif;
                transition: border-color 0.2s, color 0.2s;
            }
            .btn-secondary:hover {
                border-color: var(--accent);
                color: var(--text);
                background: transparent;
            }
            .btn-primary {
                background: linear-gradient(135deg, var(--accent), var(--bright));
                border: none;
                border-radius: 8px;
                font-family: 'Rajdhani', sans-serif;
                font-weight: 700;
                letter-spacing: 0.06em;
                box-shadow: 0 4px 14px rgba(45,125,210,0.35);
                transition: opacity 0.2s, transform 0.15s;
            }
            .btn-primary:hover {
                opacity: 0.88;
                transform: translateY(-1px);
            }
            .btn-danger {
                background: rgba(231,74,59,0.15);
                border: 1px solid rgba(231,74,59,0.4);
                color: #e74a3b;
                border-radius: 8px;
                transition: background 0.2s;
            }
            .btn-danger:hover {
                background: rgba(231,74,59,0.3);
                color: #e74a3b;
            }
            .alert-danger {
                background: rgba(231,74,59,0.12);
                border: 1px solid rgba(231,74,59,0.3);
                color: #e74a3b;
                border-radius: 10px;
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
                            <a class="nav-link active" href="gerer_bornes.php" id="navGererBornes">Gestion des bornes</a>
                        </li>
                        <li class="nav-item">
                            <a class="nav-link" href="choix_bornes.php" id="navBorne">Choix borne</a>
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
                    <div>
                        <h2>Gestion des Bornes</h2>
                        <p>Ajouter, modifier ou supprimer des bornes de recharge</p>
                    </div>
                </div>
                <div class="table-card">
                    <div>
                        <table id="table_borne" class="table table-striped table-bordered text-center">
                            <thead>
                                <tr>
                                    <th class="text-center">id</th>
                                    <th class="text-center">Nom</th>
                                    <th class="text-center">Emplacement</th>
                                    <th class="text-center">Puissance</th>
                                    <th class="text-center">IP usine</th>
                                    <th class="text-center">Actions &nbsp;<span class="text-success ms-3" id="ajouterBorne" style="cursor:pointer;">&#43;</span></th>
                                </tr>
                            </thead>
                            <tbody></tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <!-- Modal Ajouter une borne -->
        <div class="modal fade" id="addBorneModal" tabindex="-1" aria-labelledby="addBorneModalLabel" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="addBorneModalLabel">Ajouter une borne</h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                    </div>
                    <div class="modal-body">
                        <form id="addBorneForm">
                            <div class="mb-3">
                                <label for="nomBorneAdd" class="form-label">Nom de la borne</label>
                                <input type="text" class="form-control" id="nomBorneAdd" required>
                            </div>
                            <div class="mb-3">
                                <label for="emplacementAdd" class="form-label">Emplacement</label>
                                <input type="text" class="form-control" id="emplacementAdd" required>
                            </div>
                            <div class="mb-3">
                                <label for="puissanceAdd" class="form-label">Puissance</label>
                                <input type="number" class="form-control" id="puissanceAdd" required>
                            </div>
                            <div class="mb-3">
                                <label for="ipUsineAdd" class="form-label">IP usine</label>
                                <input type="text" class="form-control" id="ipUsineAdd" required>
                            </div>
                        </form>
                        <div id="addBorneError" class="alert alert-danger d-none"></div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                        <button type="button" class="btn btn-primary" id="ajoutBorne">Ajouter</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- Modal Modifier une borne -->
        <div class="modal fade" id="modBorneModal" tabindex="-1" aria-labelledby="modBorneModalLabel" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="modBorneModalLabel">Modifier une borne</h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                    </div>
                    <div class="modal-body">
                        <form id="modBorneForm">
                            <div class="mb-3">
                                <label for="nomBorneMod" class="form-label">Nom de la borne</label>
                                <input type="text" class="form-control" id="nomBorneMod" required>
                            </div>
                            <div class="mb-3">
                                <label for="emplacementMod" class="form-label">Emplacement</label>
                                <input type="text" class="form-control" id="emplacementMod" required>
                            </div>
                            <div class="mb-3">
                                <label for="puissanceMod" class="form-label">Puissance</label>
                                <input type="number" class="form-control" id="puissanceMod" required>
                            </div>
                            <div class="mb-3">
                                <label for="ipUsineMod" class="form-label">IP usine</label>
                                <input type="text" class="form-control" id="ipUsineMod" required>
                            </div>
                            <input type="hidden" id="idBorneMod"/>
                        </form>
                        <div id="modBorneError" class="alert alert-danger d-none"></div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                        <button type="button" class="btn btn-primary" id="modifBorne">Mettre à jour</button>
                    </div>
                </div>
            </div>
        </div>

        <!-- Modal Supprimer une borne -->
        <div class="modal fade" id="delBorneModal" tabindex="-1" aria-labelledby="delBorneModalLabel" aria-hidden="true">
            <div class="modal-dialog">
                <div class="modal-content">
                    <div class="modal-header">
                        <h5 class="modal-title" id="delBorneModalLabel">Supprimer une borne</h5>
                        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
                    </div>
                    <div class="modal-body">
                        <form id="delBorneForm">
                            <div class="mb-3">
                                Êtes-vous sûr de vouloir supprimer la borne <b id="supBorneTxt"></b> ?
                            </div>
                            <input type="hidden" id="idBorneSup"/>
                        </form>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-secondary" data-bs-dismiss="modal">Annuler</button>
                        <button type="button" class="btn btn-danger" id="delBorne">Supprimer</button>
                    </div>
                </div>
            </div>
        </div>

    </body>
</html>