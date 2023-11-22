#pragma once

#include "ButtonController.h"
#include <gmock/gmock.h>

class MockButtonController : public ButtonController {
public:
  MOCK_METHOD(void, registerButton, (int, std::shared_ptr<IButton>), (override));
};
