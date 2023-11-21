#include "ButtonController.h"

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  _observers.push_back(observer);
}

void ButtonController::onButtonPress(int buttonId) {
  // Logic to handle a button press
  // ...

  // Notify observers
  notifyObservers(buttonId);
}

void ButtonController::notifyObservers(int buttonId) {
  for (auto &observer : _observers) {
    if (observer) {
      observer->onButtonPress(buttonId);
    }
  }
}
