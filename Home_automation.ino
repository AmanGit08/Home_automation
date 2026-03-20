#include "DHT.h"

// Pins
#define PIR_PIN D5
#define LED_PIN D2
#define DHT_PIN D4

// Sensor Type
#define DHTTYPE DHT11   // change to DHT22 if needed

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  Serial.println("System Ready...");
}

void loop() {

  // -------- Motion Detection --------
  int motion = digitalRead(PIR_PIN);

  if (motion == LOW) {
    digitalWrite(LED_PIN, HIGH);//THIS IS THE STATE AT WHICH THE LED OF GETTING OFF
    Serial.println(" NO MOTION DECTECTED → LED OFF");
  } else {
    digitalWrite(LED_PIN, LOW);//THIS IS THE STATE THE LED IS GETTING ON
    Serial.println("   Motion DECTEDTED → LED ON");
  }

  // -------- Temperature Reading --------
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
  }

  delay(2000);  // Important for DHT stability
}