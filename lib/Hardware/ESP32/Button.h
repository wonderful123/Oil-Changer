#pragma once

#include "ButtonBase.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <Bounce2.h>

class Button : public ButtonBase {
public:
  explicit Button(const GpioPinConfig &config,
                       ButtonController &controller);

protected:
  void updatePressedState() override;
  static void handleInterrupt();

private:
  Bounce _debouncer;
  ButtonController &_controller;
  static std::unordered_map<int, ButtonESP32 *> _instanceMap;
};
#endif // PLATFORM_ESP32
