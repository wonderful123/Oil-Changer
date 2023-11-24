#pragma once

#include "ButtonBase.h"
#include <gmock/gmock.h>

class MockButton : public ButtonBase {
public:
  explicit MockButton(const HardwarePinConfig &config) : ButtonBase(config) {}

  MOCK_METHOD(void, updatePressedState, (), (override));
  MOCK_METHOD(bool, isPressed, (), (const, override));
};
