#include "DHT.h"

DHT::DHT(uint8_t pin, uint8_t type) {
  _pin = pin;
}

void DHT::begin() {
  pinMode(_pin, INPUT_PULLUP);
}

float DHT::readTemperature() {
  if (read()) return data[2];
  return NAN;
}

float DHT::readHumidity() {
  if (read()) return data[0];
  return NAN;
}

bool DHT::read() {
  uint8_t i, j;

  for (i = 0; i < 5; i++) data[i] = 0;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delay(20);

  digitalWrite(_pin, HIGH);
  delayMicroseconds(40);
  pinMode(_pin, INPUT);

  if (pulseIn(_pin, LOW) == 0) return false;
  if (pulseIn(_pin, HIGH) == 0) return false;

  for (j = 0; j < 5; j++) {
    for (i = 0; i < 8; i++) {
      while (digitalRead(_pin) == LOW);
      unsigned long t = pulseIn(_pin, HIGH);
      if (t > 40) data[j] |= (1 << (7 - i));
    }
  }

  if ((data[0] + data[1] + data[2] + data[3]) != data[4]) {
    return false;
  }

  return true;
}