
//--------- new code--------------

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include <DHT_U.h>

// Pins
#define PIR_PIN D5
#define RELAY_PIN D2
#define DHT_PIN D4
#define DHTTYPE DHT11 

DHT dht(DHT_PIN, DHTTYPE);
ESP8266WebServer server(80);

// WiFi Credentials
const char* ssid = "Airtel_BotMakers-2.4G";
const char* password = "Bot@2024";

// variables
bool autoMode = true;
bool ledState = false;

// ----------- FUNCTION DECLARATIONS -----------
String webpage();
void handleRoot();
void handleAuto();
void handleManual();
void handleOn();
void handleOff();
void action();

// ----------- SETUP -----------
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH); // relay OFF

  dht.begin();

  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // ROUTES
  server.on("/", handleRoot);
  server.on("/AUTO", handleAuto);
  server.on("/MANUAL", handleManual);
  server.on("/ON", handleOn);
  server.on("/OFF", handleOff);

  server.begin();
}

// ----------- HANDLERS -----------
void handleRoot() {
  server.send(200, "text/html", webpage());
}

void handleAuto() {
  autoMode = true;
  server.send(200, "text/html", webpage());
}

void handleManual() {
  autoMode = false;
  server.send(200, "text/html", webpage());
}

void handleOn() {
  ledState = true;
  server.send(200, "text/html", webpage());
}

void handleOff() {
  ledState = false;
  server.send(200, "text/html", webpage());
}

// ----------- SENSOR LOGIC -----------
void action() {
  int motion = digitalRead(PIR_PIN);

  if (autoMode) {
    if (motion == HIGH)
      digitalWrite(RELAY_PIN, LOW);   // ON
    else
      digitalWrite(RELAY_PIN, HIGH);  // OFF
  } else {
    if (ledState)
      digitalWrite(RELAY_PIN, LOW);
    else
      digitalWrite(RELAY_PIN, HIGH);
  }
}

// ----------- WEBPAGE -----------
String webpage() {
  float temp = dht.readTemperature();

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">

<style>
body {
  font-family: Arial;
  text-align: center;
  background: #0f172a;
  color: white;
}

.card {
  background: #1e293b;
  padding: 20px;
  margin: 20px auto;
  width: 320px;
  border-radius: 15px;
}

h1 {
  color: #38bdf8;
}

.btn {
  display: block;
  margin: 10px;
  padding: 12px;
  border-radius: 10px;
  text-decoration: none;
  color: white;
  font-size: 16px;
}

.on { background: #22c55e; }
.off { background: #ef4444; }
.auto { background: #3b82f6; }
.manual { background: #f59e0b; }

</style>
</head>

<body>

<div class="card">
  <h1>🏠 Home Automation</h1>

  <p>🌡 Temperature: %TEMP% °C</p>
  <p>⚙ Mode: %MODE%</p>

  <a class="btn auto" href="/AUTO">AUTO MODE</a>
  <a class="btn manual" href="/MANUAL">MANUAL MODE</a>
  <a class="btn on" href="/ON">TURN ON</a>
  <a class="btn off" href="/OFF">TURN OFF</a>

</div>

</body>
</html>
)rawliteral";

  html.replace("%TEMP%", String(temp));
  html.replace("%MODE%", autoMode ? "AUTO" : "MANUAL");

  return html;
}

// ----------- LOOP -----------
void loop() {
  server.handleClient();
  action();  // run sensor logic
}