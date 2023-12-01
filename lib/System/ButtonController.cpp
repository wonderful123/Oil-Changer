#include "ButtonController.h"
#include <algorithm>

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  auto it = std::find_if(
      _observers.begin(), _observers.end(),
      [&observer](const std::shared_ptr<IButtonControllerObserver> &o) {
        return o == observer;
      });
  if (it == _observers.end()) {
    _observers.push_back(observer);
  }
}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  // Check if button is already registered
  if (_buttons.find(id) != _buttons.end()) {
    return;
  }
  _buttons[id] = button;
}

void ButtonController::notifyObservers(const std::string &id) {
  // Notify observers of button press event
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(id); // Notify with button ID
    }
  }
}

void ButtonController::processButtonStates() {
  auto now = std::chrono::steady_clock::now();
  for (auto &pair : _buttonStates) {
    const std::string &id = pair.first;
    ButtonState &state = pair.second;

    state.isPressed = state.button->isPressed();
    if (state.isPressed) {
      if (!state.isInAutoRepeatMode) {
        auto elapsedSinceLastPress =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - state.lastPressTime)
                .count();
        if (elapsedSinceLastPress >=
            _settings.buttons.at(id).autoRepeat.initialDelayMs) {
          state.isInAutoRepeatMode = true;
          state.lastRepeatTime = now;
          notifyObservers(id); // Initial auto-repeat event
        }
      } else {
        auto elapsedSinceLastRepeat =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - state.lastRepeatTime)
                .count();
        if (elapsedSinceLastRepeat >=
            _settings.buttons.at(id).autoRepeat.standardRateMs) {
          state.lastRepeatTime = now;
          notifyObservers(id); // Subsequent auto-repeat events
        }
      }
    } else {
      state.isInAutoRepeatMode = false;
      state.lastPressTime = now;
    }
  }
}

std::shared_ptr<IButton>
ButtonController::getButtonById(const std::string &id) const {
  auto it = _buttons.find(id);
  if (it != _buttons.end()) {
    return it->second;
  }
  return nullptr; // Return nullptr if the button is not found
}