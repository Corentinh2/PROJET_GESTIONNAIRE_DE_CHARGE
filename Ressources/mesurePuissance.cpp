/* Calibrates the current sensor (SCT013)*/
/* ----------------------------------------------------*/
/* Instructions:
  1. Set up the hardware, the potentiometer of the voltage sensor should be already adjusted.
  2. Find an "almost pure" resistive load (no motors, no reactors, no electromagnets, no LEDs). Examples: heater, boiler, electric shower, electric oven, kettle...
  3. Install a voltmeter and ammeter to use as reference.
  4. Connect the voltage measurement and current measurement sensors.
  5. Edit the sketch *calibrate-vi.ino* and set the correct GPIO pins for the sensors.
  6. Set the calibration coefficients CV1, CV2, CV3, CI1, CI2 and CI3 to 1000 in the same file.
  7. Compile and update the code from Arduino IDE.
  8. Watch the values in the serial terminal and wait for them to stabilize. Use 115200bps as baud rate.
  9. Take a note of the measured current (I) and voltage (V) from the ESP32 and the current and voltage from the reference voltmeter (Vr) and ammeter (Ir).
  10. Calculate the calibration factors: CVnew = Vr*CVold/V, CInew = Ir*CIold/I where CVold and CIold are the previous calibrations from the sketch (initially 1000).
  11. Change the values under the "Calibration" section of the code to the calculated ones (CInew and CVnew).
  12. Compile and upload the code again, watch the serial monitor until the data stabilizes and then check if the measurements are correct.
  13. Repeat steps 8 to 12 if necessary.
*/
 
 
// Include libraries
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
 
 
// Define constants
 
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
 
// Replace with your own WiFi credentials
 
const char* ssid = "Note 10s";
const char* password = "abcd1234";
 
bool GPIO_State = 0;
const int Led_Pin = 2;
float v1 = 0;
float cur = 0;
float power =  0 ;
 
String ip = "0.0.0.0" ;
 
float  ptime  = 0 ;
 
 
JSONVar readings;
 
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
 
 
// Initialize objects
 
 
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
 
 
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#include "EmonLib.h"  // Include Emon Library
#define ESP32
 
// Pin configuration
#define V1 34
 
 
#define I1 35
 
 
// Calibration settings (allways start with 1000)
 
#define CV1 768.75
#define CI1 60.6
 
// HTML page stored in program memory
 
 
char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Power Meter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>html {
    font-family: Arial, Helvetica, sans-serif;
    display: inline-block;
    text-align: center;
}
h1 {
    font-size: 1.8rem;
    color: white;
}
.topnav {
    overflow: hidden;
    background-color: #0A1128;
}
body {
    margin: 0;
}
.content {
    padding: 50px;
}
.card-grid {
    max-width: 800px;
    margin: 0 auto;
    display: grid;
    grid-gap: 2rem;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
}
.card {
    background-color: white;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
}
.card-title {
    font-size: 1.2rem;
    font-weight: bold;
    color: #034078
}
.reading {
    font-size: 1.2rem;
    color: #1282A2;
}
  </style>
<title>ESP32 Power meter</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
 
 
        <div class="topnav">
            <h1>SENSOR READINGS (WEBSOCKET)</h1>
        </div>
        <div class="content">
            <div class="card-grid">
                <div class="card">
                    <p class="card-title"><i class="fas fa-thermometer-threequarters" style="color:#059e8a;"></i> Voltage</p>
                    <p class="reading"><span id="voltage"></span> V</p>
                </div>
                <div class="card">
                    <p class="card-title"> Current</p>
                    <p class="reading"><span id="current"></span> A</p>
                </div>
                <div class="card">
                    <p class="card-title">Power</p>
                    <p class="reading"><span id="power"></span> Watt</p>
                </div>
            </div>
        </div>
      
 
 
<script>
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
// Init web socket when the page loads
window.addEventListener('load', onload);
 
function onload(event) {
    initWebSocket();
}
 
function getReadings(){
    websocket.send("getReadings");
}
 
function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}
 
// When websocket is established, call the getReadings() function
function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}
 
function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
 
// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    console.log(event.data);
    var myObj = JSON.parse(event.data);
    var keys = Object.keys(myObj);
 
    for (var i = 0; i < keys.length; i++){
        var key = keys[i];
        document.getElementById(key).innerHTML = myObj[key];
    }
}
</script>
</body>
</html>
)rawliteral";
 
 
// Function to notify clients
 
void notifyClients() {
  ws.textAll(getSensorReadings());
}
 
 
String processor(const String& var){
  Serial.println(var);
  return "boom" ;
}
// Function to get sensor readings
 
String getSensorReadings(){  // NO1
  readings["voltage"] = String(v1);
  readings["current"] =  String(cur);
  readings["power"] = String(power);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}
 
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
      //if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      Serial.println(sensorReadings);
      notifyClients();
    //}
  }
}
 
// WebSocket event handler
 
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %sn", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnectedn", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}
// Initialize WebSocket
 
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
 
EnergyMonitor emon1;  // Phase 1
 
// Display initialization and data functions
 
 
void disinit() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      
      ;  // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  Serial.println(F("Display initialised"));
  display.clearDisplay();
  display.display();
  delay(200);  // Pause for 2 seconds
 
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);  // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);
 
  display.setCursor(0, 20);
 
  display.println(F("  Diyprojectlabs.com"));
  display.display();
  delay(5000);
 
 
  //printing the ip address to the screen
 
  
  display.setCursor(0, 48);
  display.print(ip);
  display.display();
 
}
 
 
void disdat(float x , float y) {
 
  display.clearDisplay();
  //delay(1);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("I-"));
  display.setCursor(24, 0);
  display.println(x);
  display.setCursor(115, 0);
  display.println("A");
 
 
  display.setCursor(0, 16);
  display.println(F("v-"));
  display.setCursor(24, 16);
  display.println(y);
  display.setCursor(115, 16);
  display.println("V");
 
 
  display.setCursor(1, 32);
  display.println(F("P-"));
  display.setCursor(24, 32);
  display.print(y * x);
  display.setCursor(115, 32);
  display.println("W");
 
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.print(ip);
  display.display();
}
 
 
void setup() {
  
    Serial.begin(115200);
      WiFi.softAP(ssid, password);
 
   disinit();
  disdat(0, 0);
 
   // Connect to Wi-Fi
  WiFi.begin(ssid, password);
 
 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  // Print ESP Local IP Address
  ip = WiFi.localIP().toString();
  Serial.println(ip);
  initWebSocket();
 
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", html_page, processor);
  });
 
  // Start server
  server.begin();
 
  
  /*
    Analog attenuation:
 
    ADC_0db: sets no attenuation. ADC can measure up to approximately 800 mV (1V input = ADC reading of 1088).
    ADC_2_5db: The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 1100 mV. (1V input = ADC reading of 3722).
    ADC_6db: The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 1350 mV. (1V input = ADC reading of 3033).
    ADC_11db (default): The input voltage of ADC will be attenuated, extending the range of measurement to up to approx. 2600 mV. (1V input = ADC reading of 1575).
 
  */
  analogSetPinAttenuation(V1, ADC_11db);
 
  analogSetPinAttenuation(I1, ADC_11db);
 
  // Phase 1
  emon1.voltage(V1, CV1, 1.732);  // Voltage: input pin, calibration, phase_shift
  emon1.current(I1, CI1);         // Current: input pin, calibration.
ptime = millis();
}
 
void loop() {
 
  Serial.println("------------");
 
  // Phase 1
  emon1.calcVI(120, 2000);  // Calculate all. No.of half wavelengths (crossings), time-out
  // emon1.serialprint();           // Print out all variables (realpower, apparent power, Vrms, Irms, power factor)
 
  float realPower = emon1.realPower;          // extract Real Power into variable
  float apparentPower = emon1.apparentPower;  // extract Apparent Power into variable
  float powerFactor = emon1.powerFactor;      // extract Power Factor into Variable
  float supplyVoltage = emon1.Vrms;           // extract Vrms into Variable
  float Irms = emon1.Irms;                    // extract Irms into Variable
 
  v1=supplyVoltage;
  cur = Irms;
  power =  Irms * supplyVoltage;  
  Serial.print("V1: ");
  Serial.print(supplyVoltage);
  Serial.print(", I1: ");
  Serial.println(Irms);
  disdat(Irms, supplyVoltage);
  getSensorReadings();
 
  if ((millis()-ptime) > 5000 ) {
  notifyClients();
  ptime = millis();
  }
  
  
  ws.cleanupClients();
}
