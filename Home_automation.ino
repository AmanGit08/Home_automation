#include "DHT.h"
#include <ESP8266WiFi.h>

// Pins
#define PIR_PIN D5
#define RELAY_PIN D2
#define DHT_PIN D4
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);
WiFiServer server(80);

// variables
bool autoMode = true;
bool ledState = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  dht.begin();  // ✅ FIXED

  WiFi.begin("Airtel_BotMakers-2.4G", "Bot@2024");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  WiFiClient client = server.available();

  // -------- SENSOR LOGIC ALWAYS RUNS --------
  //int motion = digitalRead(PIR_PIN);

  if (autoMode) {
    int motion = digitalRead(PIR_PIN);
    if (motion == HIGH) {
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("yes is working");
      delay(3000);
    } else {
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("not is working");
      delay(3000);
    }
    //Serial.println(digitalRead(PIR_PIN));
  //} else {
   // digitalWrite(RELAY_PIN, ledState);
  


  float temp = dht.readTemperature();  

  // -------- WEB SERVER --------
  if (client) {
    String request = client.readStringUntil('\r');

    // Mode control
  if (request.indexOf("GET /AUTO") != -1) autoMode = true;
  if (request.indexOf("GET /MANUAL") != -1) autoMode = false;
  if (request.indexOf("GET /ON") != -1) ledState = true;
  if (request.indexOf("GET /OFF") != -1) ledState = false;

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    client.println("<h1>Home Automation</h1>");
    client.println("<p>Temperature: " + String(temp) + " C</p>");
    client.println("<p>Mode: " + String(autoMode ? "AUTO" : "MANUAL") + "</p>");

    client.println("<a href=\"/AUTO\">AUTO MODE</a><br>");
    client.println("<a href=\"/MANUAL\">MANUAL MODE</a><br>");
    client.println("<a href=\"/ON\">LED ON</a><br>");
    client.println("<a href=\"/OFF\">LED OFF</a><br>");

    client.stop();
  }
}
}