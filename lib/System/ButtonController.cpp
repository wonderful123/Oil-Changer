#include "ButtonController.h"

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  _observers.push_back(observer);
}

void ButtonController::registerButton(int pinNumber,
                                      std::shared_ptr<IButton> button) {
  _buttons[pinNumber] = button;
}

void ButtonController::notifyObservers(int pinNumber) {
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(pinNumber);
    }
  }
}
