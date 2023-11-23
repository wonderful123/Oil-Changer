#pragma once

#include "ButtonBase.h"

#ifdef PLATFORM_ESP32
#include <Arduino.h>
#include <Bounce2.h>
#include <functional>

class Button : public ButtonBase {
public:
  using ButtonPressCallback = std::function<void(const std::string &)>;

  explicit Button(const GpioPinConfig &config);

protected:
  void updatePressedState() override;
  static void IRAM_ATTR handleInterrupt();

private:
  Bounce _debouncer;
  ButtonPressCallback _onPressCallback;
  static std::unordered_map<int, Button *> _instanceMap;
};
#endif // PLATFORM_ESP32
