/** @brief Broche de données du capteur DS18B20 */
#define PIN_DS18S20 18

/** @brief Port d'écoute du serveur WebSocket */
#define PORT 5555

/** @brief Température maximale en degrés Celsius avant déclenchement de l'alarme */
#define TEMPMAX 60

/** @brief Broche de réception du signal d'alarme du DS3231 */
#define PIN_ALARME 19

/** @brief Chemin de la base de données SQLite sur le système de fichiers LittleFS */
#define CHEMIN_BDD "/littlefs/programme.db"

/** @brief Broche de commande du relais */
#define PIN_RELAIS 26

#define TENSION     34      // Pin ADC tension
#define COURANT     35      // Pin ADC courant
#define CALIB       234.26  // Calibration tension
#define DEPHASAGE   1.7     // Déphasage tension
#define SENSI       60.6    // Sensibilité capteur courant


#define DEBUGETTEST true