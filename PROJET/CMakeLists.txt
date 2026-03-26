cmake_minimum_required(VERSION 3.16)

project(PROJET VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. On charge tous les modules nécessaires en une seule fois
find_package(Qt6 REQUIRED COMPONENTS Core Quick WebSockets)

qt_standard_project_setup(REQUIRES 6.8)

# Définition de l'exécutable final (le fichier binaire)
qt_add_executable(appPROJET
    main.cpp
)

# Configuration du module QML et liaison des fichiers C++
qt_add_qml_module(appPROJET
    URI PROJET
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        communicationtemp.h communicationtemp.cpp
        communicationesp.h communicationesp.cpp
        communicationraspi.h communicationraspi.cpp
        RESOURCES
        RESOURCES
        QML_FILES logique.js
        QML_FILES AppButton.qml
        QML_FILES AppTextField.qml
        QML_FILES AppCard.qml
        QML_FILES VehiculesPage.qml
        QML_FILES BootPage.qml
        QML_FILES StatusPage.qml
        QML_FILES DashboardPage.qml
        QML_FILES SettingsPage.qml
        QML_FILES SessionsPage.qml
        RESOURCES android/AndroidManifest.xml android/build.gradle android/res/values/libs.xml android/res/xml/qtprovider_paths.xml
        QML_FILES KmPage.qml
)

set_target_properties(appPROJET PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 2. On lie toutes les librairies au binaire final en un seul bloc
target_link_libraries(appPROJET
    PRIVATE Qt6::Core Qt6::Quick Qt6::WebSockets
)

include(GNUInstallDirs)
install(TARGETS appPROJET
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


set_property(TARGET appPROJET PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
