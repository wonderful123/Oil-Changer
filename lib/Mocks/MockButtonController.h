#ifdef PLATFORM_NATIVE

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
  MOCK_METHOD(void, addObserver, (std::shared_ptr<IButtonControllerObserver> observer));
  MOCK_METHOD(void, registerButton, (const std::string &id, std::shared_ptr<IButton> button));
  MOCK_METHOD(std::shared_ptr<IButton>, getButtonById, (const std::string &id), (const));
  MOCK_METHOD(void, processButtonStates, ());
  MOCK_METHOD(void, notifyObservers, (const std::string &id));
};

#endif // PLATFORM_NATIVE
