#ifndef DHT_H
#define DHT_H

#include "Arduino.h"

#define DHT11 11

class DHT {
public:
  DHT(uint8_t pin, uint8_t type);
  void begin();
  float readTemperature();
  float readHumidity();

private:
  uint8_t _pin;
  uint8_t data[5];
  bool read();
};

#endif