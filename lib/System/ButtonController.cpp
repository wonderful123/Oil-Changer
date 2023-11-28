#include "ButtonController.h"
#include <Arduino.h>
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
  _controllers[id] = this;
  button->setOnPressCallback(ButtonController::buttonCallback);
}

void registerButton(const std::string &id, std::shared_ptr<IButton> button) {}

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

std::unordered_map<std::string, ButtonController *>
    ButtonController::_controllers;

void ButtonController::buttonCallback(const std::string &id) {
  Logger::info("buttonCallback");
  delay(1000);
  if (_controllers.find(id) != _controllers.end()) {
    _controllers[id]->notifyObservers(id);
  }
}