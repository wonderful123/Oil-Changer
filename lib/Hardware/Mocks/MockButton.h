#pragma once

#ifdef PLATFORM_NATIVE

#include <gmock/gmock.h>

#include "ButtonBase.h"

class MockButton : public ButtonBase {
public:
  MockButton(const HardwarePinConfig &config) : ButtonBase(config) {
    setInitialized(true);
    _buttonPressed = false;
  }

  void simulatePress(bool buttonPressed) { _buttonPressed = buttonPressed; }

  bool isButtonPressed() override { return _buttonPressed; }

  void mockAdjustLastPressTime(std::chrono::milliseconds duration) {
    adjustLastPressTimeForTesting(duration);
  }

private:
  InteractionSettings::AutoRepeat currentSettings;
  bool _buttonPressed;
};

#endif // PLATFORM_NATIVE
