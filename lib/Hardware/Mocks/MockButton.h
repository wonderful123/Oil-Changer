#pragma once

#ifdef PLATFORM_NATIVE

#include "ButtonBase.h"
#include <gmock/gmock.h>

class MockButton : public ButtonBase {
public:
  explicit MockButton(const HardwarePinConfig &config) : ButtonBase(config) {
    setInitialized(true); // Assuming ButtonBase has setInitialized
  }
  
  MOCK_METHOD(bool, isPressed, (), (const, override));
  MOCK_METHOD(void, setOnPressCallback, (ButtonPressCallback callback), (override));
  MOCK_METHOD(void, update, (), (override));
};

#endif // PLATFORM_NATIVE
