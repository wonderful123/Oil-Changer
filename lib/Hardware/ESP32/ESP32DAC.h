// ESP32DAC.h
#pragma once

#ifdef PLATFORM_ESP32

#include "DACBase.h"
#include <Arduino.h>

class ESP32DAC : public DACBase {
public:
  explicit ESP32DAC(const HardwarePinConfig &config);

protected:
  // value is between 0 and 1
  void applyValue(float value) override;
};

#endif // PLATFORM_ESP32