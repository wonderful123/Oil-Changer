#include "ButtonController.h"

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  _observers.push_back(observer);
}

void ButtonController::registerButton(int buttonId,
                                      std::shared_ptr<ButtonBase> button) {
  _buttons[buttonId] = button;
}

void ButtonController::notifyObservers(int buttonId) {
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(buttonId);
    }
  }
}
