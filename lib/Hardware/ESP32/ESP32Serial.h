#pragma once

#ifdef PLATFORM_ESP32

#include "ISerial.h"
#include <HardwareSerial.h>

class HardwarePinConfig;

class ESP32Serial : public ISerial {
public:
  explicit ESP32Serial(const HardwarePinConfig &config);

  void begin(unsigned long baudrate) override;
  void end() override;
  size_t write(uint8_t byte) override;
  int read() override;
  int available() override;
  void flush() override;

private:
  HardwareSerial _serial;
  int _rxPin, _txPin;
};

#endif // PLATFORM_ESP32
