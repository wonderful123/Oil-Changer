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
  if (_buttons.find(id) == _buttons.end()) {
    _buttons[id] = button;
    _buttonStates[id] =
        ButtonState{button, false, std::chrono::steady_clock::now(),
                    std::chrono::steady_clock::now(), false};

    // Apply settings to the button if available
    if (_settings.buttons.find(id) != _settings.buttons.end()) {
      button->setAutoRepeatSettings(_settings.buttons.at(id).autoRepeat);
    }
  }
}

void ButtonController::notifyMediator(const std::string &id) {
  if (_mediator) {
    EventData eventData;
    eventData.id = id;
    _mediator->notify(this, EventType::BUTTON_PRESSED, &eventData);
  }
}

void ButtonController::processButtonStates() {
  auto now = std::chrono::steady_clock::now();
  for (auto &[id, state] : _buttonStates) {
    state.button->update();
    handleAutoRepeat(id, state);
  }
}

std::shared_ptr<IButton> ButtonController::getButtonById(
    const std::string &id) const {
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
                                        ButtonState &state) {
  auto now = std::chrono::steady_clock::now();

  if (state.button->isPressed()) {
    // Check for auto-repeat activation
    if (!state.isInAutoRepeatMode &&
        (now - state.lastPressTime >
         std::chrono::milliseconds(
             _settings.buttons.at(id).autoRepeat.initialDelayMs))) {
      state.isInAutoRepeatMode = true;
      state.lastRepeatTime = now;
      notifyMediator(id);  // Notify initial auto-repeat event
    } else if (state.isInAutoRepeatMode &&
               (now - state.lastRepeatTime >
                std::chrono::milliseconds(
                    _settings.buttons.at(id).autoRepeat.standardRateMs))) {
      state.lastRepeatTime = now;
      notifyMediator(id);  // Notify subsequent auto-repeat events
    }
  } else {
    state.isInAutoRepeatMode = false;
  }

  // Update the last press time
  if (state.isPressed != state.button->isPressed()) {
    state.isPressed = state.button->isPressed();
    if (state.isPressed) {
      state.lastPressTime = now;
    }
  }
}

void ButtonController::receiveEvent(EventType eventType,
                                    const EventData *eventData) {
  // Handle the event
  // Implementation depends on how ButtonController should react to events
}