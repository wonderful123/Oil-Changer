#pragma once

#ifdef PLATFORM_ESP32

#include "ButtonBase.h"
#include <Bounce2.h>

class ESP32Button : public ButtonBase {
public:
  explicit ESP32Button(const HardwarePinConfig &config,
                       unsigned long debounceDuration = 50);

  void updateButtonState() override;  // Update button state

 private:
  Bounce _debouncer;
};

#endif // PLATFORM_ESP32
