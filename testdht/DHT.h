#ifndef DHT_H
#define DHT_H

#include "Arduino.h"

#define DHT11 11
#define DHT22 22

class DHT {
public:
  DHT(uint8_t pin, uint8_t type);
  void begin();
  float readTemperature();
  float readHumidity();

private:
  uint8_t _pin, _type;
  uint8_t data[5];

  bool read();
};

#endif
