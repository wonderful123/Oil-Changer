#include "ButtonController.h"

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  _observers.push_back(observer);
}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  _buttons[id] = button;
}

void ButtonController::notifyObservers(const std::string &id) {
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(id); // Notify with button ID
    }
  }
}
