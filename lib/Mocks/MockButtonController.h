#pragma once

#include "ButtonController.h"
#include <gmock/gmock.h> // Include Google Mock

class MockButtonController : public ButtonController {
public:
  explicit MockButtonController(std::shared_ptr<IMediator> mediator)
      : ButtonController(mediator) {}

  // Override virtual methods with MOCK_METHOD
  MOCK_METHOD(void, registerButton,
              (const std::string &id, std::shared_ptr<IButton> button),
              (override));
  MOCK_METHOD(void, processButtonStates, (), (override));
  MOCK_METHOD(std::shared_ptr<IButton>, getButtonById, (const std::string &id),
              (const, override));
  MOCK_METHOD(void, setInteractionSettings,
              (const InteractionSettings &settings), (override));
  MOCK_METHOD(void, receiveEvent,
              (EventType eventType, const EventData *eventData), (override));
};
