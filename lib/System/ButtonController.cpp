// ButtonController.cpp

#include "ButtonController.h"
#include "FSM/StateMachine.h"
#include "InteractionSettings.h"
#include "Logger.h"

std::unordered_map<std::string, ButtonId> buttonIdMap = {
    {"ButtonPlus", ButtonId::Plus},
    {"ButtonMinus", ButtonId::Minus},
    {"ButtonStart", ButtonId::Start},
    {"ButtonStop", ButtonId::Stop},
    {"ButtonManualExtract", ButtonId::ManualExtract},
    {"ButtonManualFill", ButtonId::ManualFill}};

ButtonController::ButtonController(
    std::shared_ptr<InteractionSettings> &settings)
    : _interactionSettings(settings) {}

void ButtonController::registerButton(std::shared_ptr<IButton> button) {
  // Check if the idString is valid
  auto idIter = buttonIdMap.find(button->idString());
  if (idIter == buttonIdMap.end()) {
    Logger::warn("[ButtonController] Invalid button ID string: " +
                 button->idString());
    return;
  }
  auto id = idIter->second;

  // Check if the button is already registered
  auto buttonIter = _buttons.find(id);
  if (buttonIter != _buttons.end()) {
    Logger::warn("[ButtonController] Attempted to re-register button: " +
                 button->idString());
    return;
  }

  // Initialize settings
  _previousButtonStates[id] = IButton::State::Released;
  auto &autoRepeatSettings = _interactionSettings->commonSettings.autoRepeat;
  button->setHoldDurationThreshold(autoRepeatSettings.initialDelayMs);
  button->setLongHoldDurationThreshold(
      autoRepeatSettings.acceleration.startAfterMs);

  // Register the button
  _buttons[id] = button;
  Logger::info("[ButtonController] Registered: " + button->idString());
}

void ButtonController::processButtonStates() {
  for (const auto &pair : _buttons) {
    const auto &button = pair.second;
    const auto idString = button->idString();
    const auto id = buttonIdMap[idString];

    button->updateButtonState();

    auto previousState = _previousButtonStates[id];
    auto currentState = button->getState();

    auto stateToString = [](IButton::State state) -> std::string {
      switch (state) {
      case IButton::State::Released:
        return std::string("Released");
      case IButton::State::Pressed:
        return std::string("Pressed");
      case IButton::State::Held:
        return std::string("Held");
      default:
        return std::string("HeldLong");
      }
    };

    // Check for state change
    if (previousState != currentState) {
      switch (currentState) {
      case IButton::State::Pressed:
        sendFSMEvent(ButtonPressEvent(idString));
        break;
      case IButton::State::Released:
        sendFSMEvent(ButtonReleaseEvent(idString));
        break;
      case IButton::State::Held:
        sendFSMEvent(ButtonHoldEvent(idString));
        break;
      case IButton::State::HeldLong:
        sendFSMEvent(ButtonLongHoldEvent(idString));
        break;
      }

      _previousButtonStates[id] = currentState; // Update previous state
    }
  }
}

std::shared_ptr<IButton>
ButtonController::getButtonById(const std::string &idString) const {
  auto iter = buttonIdMap.find(idString);
  if (iter != buttonIdMap.end()) {
    ButtonId id = iter->second;
    auto it = _buttons.find(id);
    // If the button is found, return the associated IButton shared pointer
    return it != _buttons.end() ? it->second : nullptr;
  }
  Logger::warn("[ButtonController] Invalid button ID: " + idString);
  return nullptr;
}