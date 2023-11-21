#include "ButtonController.h"
#include <chrono>

std::unordered_map<int, int> ButtonController::_buttonPinMap;
std::unordered_map<int, ButtonController *> ButtonController::_instances;

ButtonController::ButtonController() {}

void ButtonController::addObserver(
    std::shared_ptr<IButtonControllerObserver> observer) {
  _observers.push_back(observer);
}

void ButtonController::registerButton(int buttonId, int pin) {
  _buttonPinMap[buttonId] = pin;
  _instances[buttonId] = this;

  // Setup pin as input and attach interrupt
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), ButtonController::handleInterrupt,
                  CHANGE);
}
void ButtonController::handleInterrupt() {
  static std::chrono::steady_clock::time_point last_interrupt_time;
  auto now = std::chrono::steady_clock::now();

  if (now - last_interrupt_time >
      std::chrono::milliseconds(50)) { // Debounce time
    last_interrupt_time = now;

    // Determine which button caused the interrupt
    for (const auto &[id, pin] : _buttonPinMap) {
      if (digitalRead(pin) == LOW) { // Assuming active LOW buttons
        _instances[id]->notifyObservers(id);
        break;
      }
    }
  }

  void ButtonController::notifyObservers(int buttonId) {
    for (auto &observer : _observers) {
      if (observer) {
        observer->onButtonPress(buttonId);
      }
    }
  }
