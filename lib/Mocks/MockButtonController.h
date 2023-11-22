#pragma once

#include "ButtonController.h"
#include <gmock/gmock.h>

class MockButtonController : public ButtonController {
public:
  MOCK_METHOD(void, registerButton,
              (const std::string &, std::shared_ptr<IButton>), (override));
};
