#include "ButtonController.h"

#include <algorithm>

#include "IButton.h"
#include "InteractionSettings.h"
#include "Mediator/ConcreteMediator.h"

ButtonController::ButtonController(std::shared_ptr<IMediator> mediator)
    : IColleague(mediator), _mediator(mediator) {
  _mediator->registerColleague(this);
}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  // Check if the button is already registered
  if (_buttons.find(id) == _buttons.end()) {
    // Add the button
    _buttons[id] = button;

    // Set a callback for the button
    button->setOnPressCallback([this](const std::string &buttonId) {
      this->handleButtonPress(buttonId);
    });

    // Apply settings to the button if available
    if (_settings.buttons.find(id) != _settings.buttons.end()) {
      button->setAutoRepeatSettings(_settings.buttons.at(id).autoRepeat);
    }

    Logger::info("[ButtonController] Registered: " + id);
  }
}

void ButtonController::handleButtonPress(const std::string &id) {
  notifyMediator(id, EventType::BUTTON_PRESSED);
  Logger::info("ButtonController::handleButtonPress: Button pressed: " + id);
}

void ButtonController::notifyMediator(const std::string &id,
                                      EventType eventType) {
  if (_mediator) {
    EventData eventData;
    eventData.id = id;
    _mediator->notify(this, eventType, &eventData);
  }
}

void ButtonController::processButtonStates() {
  for (const auto &pair : _buttons) {
    const auto &id = pair.first;
    auto &button = pair.second;

    button->update(); // Update the state of the button
    auto state = button->getCurrentState(); // Get the current state
    handleAutoRepeat(id, state); // Handle auto-repeat logic if necessary
  }
}

std::shared_ptr<IButton>
ButtonController::getButtonById(const std::string &id) const {
  auto it = _buttons.find(id);
  return it != _buttons.end() ? it->second : nullptr;
}

void ButtonController::setInteractionSettings(
    const InteractionSettings &settings) {
  _settings = settings;
  // Update settings for each registered button
  for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
    auto id = it->first;
    auto &button = it->second;
    auto buttonSettingsIter = _settings.buttons.find(id);
    if (buttonSettingsIter != _settings.buttons.end()) {
      button->setAutoRepeatSettings(buttonSettingsIter->second.autoRepeat);
    }
  }
}

void ButtonController::handleAutoRepeat(const std::string &id,
                                        const IButton::ButtonState &state) {
  if (state.isPressed && state.isInAutoRepeatMode) {
    // If the button is pressed and in auto-repeat mode, notify the mediator
    //    notifyMediator(id, EventType::BUTTON_AUTO_REPEAT);
    Logger::info("[ButtonController] handleAutoRepeat: " + id + " " +
                 std::to_string(state.isPressed) + " " +
                 std::to_string(state.isInAutoRepeatMode));
  }
}

void ButtonController::receiveEvent(EventType eventType,
                                    const EventData *eventData) {
  // Handle the event
  // Implementation depends on how ButtonController should react to events
}