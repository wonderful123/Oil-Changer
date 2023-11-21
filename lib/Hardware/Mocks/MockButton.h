#pragma once

#include "ButtonBase.h"
#include <gmock/gmock.h>

class MockButton : public ButtonBase {
public:
  explicit MockButton(const GpioPinConfig &config) : ButtonBase(config) {}

  MOCK_METHOD(void, updatePressedState, (), (override));
};
