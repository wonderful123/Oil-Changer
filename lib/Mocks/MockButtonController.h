#pragma once

#include "IButtonControllerObserver.h"
#include "ButtonController.h"
#include "IButton.h"
#include <gmock/gmock.h>

class MockButtonControllerObserver : public IButtonControllerObserver {
public:
  MOCK_METHOD(void, onButtonPress, (const std::string &id), (override));
  MOCK_METHOD(void, update, (), (override));
};

class MockButtonController : public ButtonController {
public:
  MOCK_METHOD(void, registerButton,
              (const std::string &, std::shared_ptr<IButton>), (override));
};
