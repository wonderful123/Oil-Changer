#pragma once

#include "DACBase.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>

class DAC : public DACBase {
public:
  explicit DAC(const GpioPinConfig &config);

protected:
  void applyValue(int value) override;
};
#endif // PLATFORM_ESP32