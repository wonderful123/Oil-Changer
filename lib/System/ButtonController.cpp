#include "ButtonController.h"
#include <algorithm>

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  if (std::find(_observers.begin(), _observers.end(), observer) ==
      _observers.end()) {
    _observers.push_back(observer);
  }
}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  _buttons[id] = button;
  _buttonStates[id] =
      ButtonState{button, false, std::chrono::steady_clock::now(),
                  std::chrono::steady_clock::now(), false};
}

void ButtonController::notifyObservers(const std::string &id) {
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(id);
    }
  }
}

void ButtonController::processButtonStates() {
  auto now = std::chrono::steady_clock::now();
  for (auto &[id, state] : _buttonStates) {
    state.button->update();

    // Update the press state and check auto-repeat
    if (state.button->isPressed()) {
      if (!state.isInAutoRepeatMode &&
          (now - state.lastPressTime >
           std::chrono::milliseconds(
               _settings.buttons.at(id).autoRepeat.initialDelayMs))) {
        state.isInAutoRepeatMode = true;
        state.lastRepeatTime = now;
        notifyObservers(id); // Notify initial auto-repeat event
      } else if (state.isInAutoRepeatMode &&
                 (now - state.lastRepeatTime >
                  std::chrono::milliseconds(
                      _settings.buttons.at(id).autoRepeat.standardRateMs))) {
        state.lastRepeatTime = now;
        notifyObservers(id); // Notify subsequent auto-repeat events
      }
    } else {
      state.isInAutoRepeatMode = false;
    }

    // Update last press time
    if (state.isPressed != state.button->isPressed()) {
      state.isPressed = state.button->isPressed();
      if (state.isPressed) {
        state.lastPressTime = now;
      }
    }
  }
}

std::shared_ptr<IButton>
ButtonController::getButtonById(const std::string &id) const {
  auto it = _buttons.find(id);
  return it != _buttons.end() ? it->second : nullptr;
}
