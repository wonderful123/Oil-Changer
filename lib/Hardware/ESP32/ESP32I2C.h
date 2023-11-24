#pragma once

#ifdef PLATFORM_ESP32

#include "II2C.h"
#include "HardwarePinConfig.h"

class ESP32I2C : public II2C {
public:
  explicit ESP32I2C(const HardwarePinConfig &config);

  void begin() override;
  void end() override;
  void beginTransmission(uint8_t address) override;
  uint8_t endTransmission(bool sendStop = true) override;
  size_t write(uint8_t value) override;
  int read() override;
  int available() override;

private:
  int _sdaPin, _sclPin;
};

#endif // PLATFORM_ESP32
