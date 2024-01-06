// ESP32I2C.cpp

#include "ESP32I2C.h"

#ifdef PLATFORM_ESP32

#include <Wire.h>

ESP32I2C::ESP32I2C(const HardwarePinConfig &config)
    : II2C(config), _sdaPin(config.pinNumber), // Assuming pinNumber is SDA
      _sclPin(-1) { // SCL pin not specified in this example
  setInitialized(true);
}

void ESP32I2C::begin() {
  if (_sclPin > -1) {
    Wire.begin(_sdaPin, _sclPin);
  } else {
    Wire.begin();
  }
}

void ESP32I2C::end() { Wire.end(); }

void ESP32I2C::beginTransmission(uint8_t address) {
  Wire.beginTransmission(address);
}

uint8_t ESP32I2C::endTransmission(bool sendStop) {
  return Wire.endTransmission(sendStop);
}

size_t ESP32I2C::write(uint8_t value) { return Wire.write(value); }

int ESP32I2C::read() { return Wire.read(); }

int ESP32I2C::available() { return Wire.available(); }

#endif // PLATFORM_ESP32
