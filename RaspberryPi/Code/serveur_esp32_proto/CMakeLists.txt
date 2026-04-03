cmake_minimum_required(VERSION 3.16)

# Nom de ton projet
project(serveur_esp32_proto VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON) # Indispensable pour Q_OBJECT
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 1. On cherche les paquets Qt6 nécessaires
# Core : Base de Qt
# Network : Gestion réseau
# WebSockets : Pour la communication avec l'ESP32
# Sql : Pour la connexion à MariaDB
find_package(Qt6 REQUIRED COMPONENTS Core Network WebSockets Sql)

# 2. On déclare l'exécutable et on liste TOUS les fichiers du projet
# C'est cette ligne qui définit la "cible" (target) pour la suite
add_executable(serveur_esp32_proto
    main.cpp
    serveur.h serveur.cpp
)

# 3. On lie les bibliothèques Qt à ton exécutable
# On utilise le nom de la cible défini juste au-dessus
target_link_libraries(serveur_esp32_proto PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::WebSockets
    Qt6::Sql
)

# Optionnel : permet d'afficher les messages de debug dans la console
if(WIN32)
    set_target_properties(serveur_esp32_proto PROPERTIES WIN32_EXECUTABLE FALSE)
endif()
