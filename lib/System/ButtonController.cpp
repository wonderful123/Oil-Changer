#include "ButtonController.h"
#include <algorithm>

ButtonController::ButtonController() {}

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
  _buttons[id] = button;
  button->setOnPressCallback(
      [this, id](const std::string &) { this->notifyObservers(id); });
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
