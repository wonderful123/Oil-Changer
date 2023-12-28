#include "ButtonController.h"
#include "Logger.h"

ButtonController::ButtonController(const InteractionSettings &settings)
    : _settings(settings) {}

void ButtonController::registerButton(const std::string &id,
                                      std::shared_ptr<IButton> button) {
  if (_buttons.find(id) == _buttons.end()) {
    _buttons[id] = button;
    applySettingsToButton(button, id);
    Logger::info("[ButtonController] Registered: " + id);
  }
}

void ButtonController::applySettingsToButton(std::shared_ptr<IButton> button,
                                             const std::string &id) {
  auto settingsIter = _settings.buttons.find(id);
  if (settingsIter != _settings.buttons.end()) {
    // If your IButton class supports setting auto-repeat settings, apply them
    // here. button->setAutoRepeatSettings(settingsIter->second.autoRepeat);
  }
}

void ButtonController::processButtonStates() {
  for (const auto &pair : _buttons) {
    const auto &id = pair.first;
    auto &button = pair.second;

    auto prevState = button->getPreviousState(); // Ensure this method is
                                                 // implemented in IButton
    auto currentState = button->getCurrentState();

    if (!prevState.isPressed && currentState.isPressed) {
      notify("button_pressed", id);
    } else if (prevState.isPressed && !currentState.isPressed) {
      notify("button_released", id);
    }
    // Handle other states and transitions as needed
  }
}

void ButtonController::notify(const std::string &event, const std::string &id) {
  for (const auto &observer : observers) {
    observer->onNotify(event, id);
  }
}

void ButtonController::attach(std::shared_ptr<IObserver> observer) {
  observers.push_back(observer);
}

void ButtonController::detach(std::shared_ptr<IObserver> observer) {
  observers.erase(std::remove(observers.begin(), observers.end(), observer),
                  observers.end());
}

std::shared_ptr<IButton>
ButtonController::getButtonById(const std::string &id) const {
  auto it = _buttons.find(id);
  return it != _buttons.end() ? it->second : nullptr;
}
