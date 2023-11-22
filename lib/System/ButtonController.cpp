#include "ButtonController.h"

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  if (std::find(_observers.begin(), _observers.end(), observer) ==
      _observers.end()) {
    // Observer is unique
    _observers.push_back(observer);
  }
}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  _buttons[id] = button;
}

void ButtonController::notifyObservers(const std::string &id) {
  // Check if button is registered
  if (_buttons.find(id) == _buttons.end()) {
    return;
  }
  // Notify observers of button press event
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(id); // Notify with button ID
    }
  }
}

void ButtonController::checkButtonStates() {
  for (const auto &buttonPair : _buttons) {
    const std::string &id = buttonPair.first;
    std::shared_ptr<IButton> button = buttonPair.second;
    if (button->isPressed()) {
      notifyObservers(id);
    }
  }
}
