#pragma once

#ifdef PLATFORM_ESP32

#include "DACBase.h"
#include <Arduino.h>

class ESP32DAC : public DACBase {
public:
  explicit ESP32DAC(const HardwarePinConfig &config);

protected:
  void applyValue(int value) override;
};

#endif // PLATFORM_ESP32