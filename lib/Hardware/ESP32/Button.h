#pragma once

#include "ButtonBase.h"
#ifdef PLATFORM_ESP32
#include <Arduino.h>

class Button : public ButtonBase {
public:
  explicit Button(const GpioPinConfig &config);

protected:
  void updatePressedState() override;
};
#endif // PLATFORM_ESP32
