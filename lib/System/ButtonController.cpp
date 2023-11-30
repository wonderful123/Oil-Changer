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
  for (const auto &buttonPair : _buttons) {
    const std::string &id = buttonPair.first;
    std::shared_ptr<IButton> button = buttonPair.second;

    bool wasPressed = button->isPressed();
    button->update(); // Update button state

    // Notify if the button was not pressed before and is pressed now
    if (!wasPressed && button->isPressed()) {
      notifyObservers(id);
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