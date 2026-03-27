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

        h1 { text-transform: uppercase; letter-spacing: 2px; margin-bottom: 10px; }
        p { color: #b0b0b0; margin-bottom: 40px; }

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
            transition: transform 0.2s, border 0.2s;
            cursor: pointer;
            border: 1px solid transparent;
            position: relative;
        }

        .borne-card:hover {
            transform: translateY(-5px);
            border-color: #00d4ff;
        }

        .borne-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 15px;
        }

        .borne-title { font-weight: bold; font-size: 1.1em; }
        .borne-date { font-size: 0.8em; color: #777; }

        .status { display: flex; align-items: center; font-size: 0.9em; margin-bottom: 10px; }
        .dot { height: 10px; width: 10px; border-radius: 50%; display: inline-block; margin-right: 8px; }
        
        .active .dot { background-color: #2ecc71; }
        .charging .dot { background-color: #f1c40f; }
        .offline .dot { background-color: #e74c3c; }

        .power { color: #00d4ff; font-weight: bold; font-size: 1.2em; position: absolute; right: 20px; bottom: 45px; }
        .alert { color: #e74c3c; font-size: 0.85em; background: rgba(231, 76, 60, 0.1); padding: 4px 8px; border-radius: 4px; }
        .signal { font-size: 0.8em; color: #555; text-align: right; margin-top: 15px; }

        .btn-accueil {
            margin-top: 50px;
            background: #2a2a2a;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 4px;
            cursor: pointer;
            text-decoration: none;
            display: inline-block;
        }
    </style>
</head>
<body>

    <h1>Sélection de la borne à superviser</h1>
    <p>Liste des bornes enregistrées — cliquez sur une borne pour consulter ses détails</p>

    <div class="container">
        <div class="borne-card" onclick="window.location.href='index.php?action=EnregistreGraph&id=1'">
            <div class="borne-header">
                <span class="borne-title">Borne #01 - Parking Nord</span>
                <span class="borne-date">2026-03-05 08:58</span>
            </div>
            <div class="status active"><span class="dot"></span> Active</div>
            <div class="alert" style="visibility: hidden;">Aucune alerte</div>
            <div class="power">7400 W</div>
            <div class="signal">Signal: OK</div>
        </div>

        <div class="borne-card" onclick="window.location.href='index.php?action=EnregistreGraph&id=2'">
            <div class="borne-header">
                <span class="borne-title">Borne #02 - Zone VIP</span>
                <span class="borne-date">2026-03-05 09:02</span>
            </div>
            <div class="status charging"><span class="dot"></span> En charge</div>
            <div class="alert">1 alerte(s)</div>
            <div class="power">12000 W</div>
            <div class="signal">Signal: OK</div>
        </div>

        <div class="borne-card" onclick="window.location.href='index.php?action=EnregistreGraph&id=3'">
            <div class="borne-header">
                <span class="borne-title">Borne #03 - Entrée Sud</span>
                <span class="borne-date">2026-03-05 07:40</span>
            </div>
            <div class="status offline"><span class="dot"></span> Hors service</div>
            <div class="alert">2 alerte(s)</div>
            <div class="power">0 W</div>
            <div class="signal">Signal: N/A</div>
        </div>

        <div class="borne-card" onclick="window.location.href='index.php?action=EnregistreGraph&id=4'">
            <div class="borne-header">
                <span class="borne-title">Borne #04 - Quai A</span>
                <span class="borne-date">2026-03-05 09:05</span>
            </div>
            <div class="status active"><span class="dot"></span> Active</div>
            <div class="alert" style="visibility: hidden;">Aucune alerte</div>
            <div class="power">4500 W</div>
            <div class="signal">Signal: OK</div>
        </div>
    </div>

    <a href="index.php" class="btn-accueil">🏠 Accueil</a>

</body>
</html>