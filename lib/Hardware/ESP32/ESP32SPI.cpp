#ifdef PLATFORM_ESP32

#include "ESP32SPI.h"
#include <Arduino.h>
#include <SPI.h>

ESP32SPI::ESP32SPI(const HardwarePinConfig &config)
    : ISPI(config), _mosiPin(config.pins.at("MOSI")),
      _misoPin(config.pins.at("MISO")), _sclkPin(config.pins.at("SCLK")),
      _csPin(config.pins.at("CS")) {
  setInitialized(true); // Set initialized flag to true
}

void ESP32SPI::begin() { SPI.begin(_sclkPin, _misoPin, _mosiPin, _csPin); }

void ESP32SPI::end() { SPI.end(); }

uint8_t ESP32SPI::transfer(uint8_t data) { return SPI.transfer(data); }

void ESP32SPI::transfer(void *buf, size_t count) { SPI.transfer(buf, count); }

#endif // PLATFORM_ESP32
