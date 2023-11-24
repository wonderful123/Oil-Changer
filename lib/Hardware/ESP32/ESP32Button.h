#pragma once

#ifdef PLATFORM_ESP32

#include "ButtonBase.h"

#include <Arduino.h>
#include <Bounce2.h>
#include <functional>

class ESP32Button : public ButtonBase {
public:
  using ButtonPressCallback = std::function<void(const std::string &)>;

  explicit ESP32Button(const HardwarePinConfig &config);

protected:
  void updatePressedState() override;
  static void IRAM_ATTR handleInterrupt();

private:
  Bounce _debouncer;
  ButtonPressCallback _onPressCallback;
  static std::unordered_map<int, ESP32Button *> _instanceMap;
};
#endif // PLATFORM_ESP32
