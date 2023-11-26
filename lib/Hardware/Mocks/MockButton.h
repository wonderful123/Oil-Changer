#pragma once

#ifdef PLATFORM_NATIVE

#include "ButtonBase.h"
#include <gmock/gmock.h>

class MockButton : public ButtonBase {
public:
  explicit MockButton(const HardwarePinConfig &config) : ButtonBase(config) {
    setInitialized(true); // Set initialized flag to true
  }

  MOCK_METHOD(void, updatePressedState, (), (override));
  MOCK_METHOD(bool, isPressed, (), (const, override));
};

#endif // PLATFORM_NATIVE