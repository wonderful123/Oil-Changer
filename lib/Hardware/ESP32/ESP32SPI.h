// ESP32SPI.h
#pragma once

#ifdef PLATFORM_ESP32

#include "ISPI.h"

class HardwarePinConfig;

class ESP32SPI : public ISPI {
public:
  explicit ESP32SPI(const HardwarePinConfig &config);

  void begin() override;
  void end() override;
  uint8_t transfer(uint8_t data) override;
  void transfer(void *buf, size_t count) override;

private:
  int _mosiPin, _misoPin, _sclkPin, _csPin;
};

#endif // PLATFORM_ESP32