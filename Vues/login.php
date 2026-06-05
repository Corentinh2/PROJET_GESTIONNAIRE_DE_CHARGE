<?php
session_start();
if (isset($_SESSION['connecte']) && $_SESSION['connecte'] === true) {
    if ($_SESSION['utilisateur'] == 'admin') {
        header("Location: admin.php");
    } else {
        header("Location: choix_bornes.php");
    }
    exit();
}
?>
<!DOCTYPE html>
<html>
    <head>
        <title>Login</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link href="js/libs/bootstrap/css/bootstrap.min.css" rel="stylesheet" />
        <script src="js/libs/jquery/jquery.min.js"></script>
        <script src="login.js"></script>
        <link href="https://fonts.googleapis.com/css2?family=Rajdhani:wght@500;700&family=DM+Sans:wght@300;400;500&display=swap" rel="stylesheet">
        <style>
            :root {
                --navy:    #0a1628;
                --blue:    #1a3a6b;
                --accent:  #2d7dd2;
                --bright:  #4fa3e8;
                --glass:   rgba(255,255,255,0.04);
                --border:  rgba(77,163,232,0.18);
                --text:    #e8eef8;
                --muted:   #7a9bbf;
            }
            *, *::before, *::after {
                box-sizing: border-box;
                margin: 0;
                padding: 0;
            }
            html, body {
                height: 100%;
                background: var(--navy);
                font-family: 'DM Sans', sans-serif;
                color: var(--text);
                overflow: hidden;
            }
            /* Animated background grid */
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
            }
            @keyframes gridScroll {
                0%   {
                    background-position: 0 0;
                }
                100% {
                    background-position: 40px 40px;
                }
            }
            /* Glow blobs */
            .blob {
                position: fixed;
                border-radius: 50%;
                filter: blur(80px);
                opacity: 0.18;
                pointer-events: none;
            }
            .blob-1 {
                width: 500px;
                height: 500px;
                background: var(--accent);
                top: -150px;
                left: -150px;
            }
            .blob-2 {
                width: 350px;
                height: 350px;
                background: var(--bright);
                bottom: -100px;
                right: -80px;
            }

            .login-wrap {
                position: relative;
                z-index: 10;
                height: 100vh;
                display: flex;
                align-items: center;
                justify-content: center;
            }
            .login-card {
                width: 420px;
                background: var(--glass);
                border: 1px solid var(--border);
                border-radius: 20px;
                padding: 48px 44px;
                backdrop-filter: blur(12px);
                box-shadow: 0 8px 48px rgba(0,0,0,0.45), inset 0 1px 0 rgba(255,255,255,0.06);
                animation: fadeUp 0.6s ease both;
            }
            @keyframes fadeUp {
                from {
                    opacity: 0;
                    transform: translateY(24px);
                }
                to   {
                    opacity: 1;
                    transform: translateY(0);
                }
            }
            .brand {
                text-align: center;
                margin-bottom: 36px;
            }
            .brand-icon {
                width: 56px;
                height: 56px;
                background: linear-gradient(135deg, var(--accent), var(--bright));
                border-radius: 16px;
                display: inline-flex;
                align-items: center;
                justify-content: center;
                font-size: 1.6rem;
                margin-bottom: 14px;
                box-shadow: 0 4px 20px rgba(45,125,210,0.4);
            }
            .brand h1 {
                font-family: 'Rajdhani', sans-serif;
                font-size: 1.9rem;
                font-weight: 700;
                letter-spacing: 0.04em;
                color: #fff;
            }
            .brand p {
                font-size: 0.82rem;
                color: var(--muted);
                margin-top: 4px;
                letter-spacing: 0.06em;
                text-transform: uppercase;
            }
            .form-label-custom {
                font-size: 0.75rem;
                font-weight: 500;
                letter-spacing: 0.08em;
                text-transform: uppercase;
                color: var(--muted);
                margin-bottom: 7px;
                display: block;
            }
            .field {
                margin-bottom: 20px;
            }
            .input-custom {
                width: 100%;
                background: rgba(255,255,255,0.05);
                border: 1px solid var(--border);
                border-radius: 10px;
                color: var(--text);
                padding: 12px 16px;
                font-family: 'DM Sans', sans-serif;
                font-size: 0.95rem;
                transition: border-color 0.2s, box-shadow 0.2s;
                outline: none;
            }
            .input-custom::placeholder {
                color: rgba(122,155,191,0.5);
            }
            .input-custom:focus {
                border-color: var(--accent);
                box-shadow: 0 0 0 3px rgba(45,125,210,0.18);
                background: rgba(255,255,255,0.07);
            }
            .input-custom.error {
                border-color: #e74c5e;
                box-shadow: 0 0 0 3px rgba(231,76,94,0.15);
            }
            .btn-login {
                width: 100%;
                padding: 13px;
                border-radius: 10px;
                border: none;
                background: linear-gradient(135deg, var(--accent) 0%, var(--bright) 100%);
                color: #fff;
                font-family: 'Rajdhani', sans-serif;
                font-size: 1.05rem;
                font-weight: 700;
                letter-spacing: 0.08em;
                text-transform: uppercase;
                cursor: pointer;
                transition: opacity 0.2s, transform 0.15s, box-shadow 0.2s;
                box-shadow: 0 4px 18px rgba(45,125,210,0.35);
                margin-top: 8px;
            }
            .btn-login:hover {
                opacity: 0.9;
                transform: translateY(-1px);
                box-shadow: 0 6px 24px rgba(45,125,210,0.5);
            }
            .btn-login:active {
                transform: translateY(0);
            }
            .divider {
                display: flex;
                align-items: center;
                gap: 12px;
                margin: 22px 0 18px;
                color: var(--muted);
                font-size: 0.78rem;
            }
            .divider::before, .divider::after {
                content: '';
                flex: 1;
                height: 1px;
                background: var(--border);
            }
            .btn-reset {
                width: 100%;
                padding: 11px;
                border-radius: 10px;
                border: 1px solid var(--border);
                background: transparent;
                color: var(--muted);
                font-family: 'DM Sans', sans-serif;
                font-size: 0.9rem;
                cursor: pointer;
                transition: border-color 0.2s, color 0.2s;
            }
            .btn-reset:hover {
                border-color: var(--accent);
                color: var(--text);
            }
            .result-row {
                display: flex;
                align-items: center;
                justify-content: space-between;
                margin-top: 16px;
            }
            .link-inscription {
                color: var(--bright);
                text-decoration: none;
                font-size: 0.85rem;
                transition: color 0.2s;
            }
            .link-inscription:hover {
                color: #fff;
            }
            .badge {
                padding: 4px 10px;
                border-radius: 6px;
                font-size: 0.78rem;
            }
            .text-bg-light {
                background: rgba(255,255,255,0.08) !important;
                color: var(--muted) !important;
            }
        </style>
    </head>
    <body>
        <div class="blob blob-1"></div>
        <div class="blob blob-2"></div>
        <div class="login-wrap">
            <div class="login-card">
                <div class="brand">
                    <div class="brand-icon">⚡</div>
                    <h1>GestioBorne</h1>
                    <p>Plateforme de supervision</p>
                </div>
                <form id="formulaireLogin">
                    <div class="field">
                        <label class="form-label-custom">Identifiant</label>
                        <input class="input-custom" type="text" id="login" name="login" placeholder="Votre login" required/>
                    </div>
                    <div class="field">
                        <label class="form-label-custom">Mot de passe</label>
                        <input class="input-custom" type="password" id="mdp" name="mdp" placeholder="••••••••" required/>
                    </div>
                    <button class="btn-login" type="button" id="verifLogin">Connexion</button>
                    <div class="divider">ou</div>
                    <button class="btn-reset" type="reset" id="reset">Réinitialiser</button>
                    <div class="result-row">
                        <a href="inscription.php" class="link-inscription">Pas encore de compte ? S'inscrire</a>
                        <span id="pastille" class="badge text-bg-light">&nbsp;&nbsp;</span>
                    </div>
                </form>
            </div>
        </div>
    </body>
</html>