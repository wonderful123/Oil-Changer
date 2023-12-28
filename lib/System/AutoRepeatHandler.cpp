// AutoRepeatHandler.cpp
#include "AutoRepeatHandler.h"
#include "Logger.h"

AutoRepeatHandler::AutoRepeatHandler(
    std::shared_ptr<ButtonController> controller,
    const InteractionSettings &settings)
    : _controller(controller), _settings(settings) {}

void AutoRepeatHandler::onNotify(const std::string &event,
                                 const std::string &buttonId) {
  if (event == "button_pressed") {
    handleButtonPressed(buttonId);
  } else if (event == "button_released") {
    handleButtonReleased(buttonId);
  }
}

void AutoRepeatHandler::handleButtonPressed(const std::string &buttonId) {
  if (_settings.buttons.find(buttonId) != _settings.buttons.end() &&
      _settings.buttons[buttonId].hasAutoRepeat) {
    _lastPressTime[buttonId] = std::chrono::steady_clock::now();
    _cachedAutoRepeatSettings[buttonId] =
        _settings.commonSettings.autoRepeat;
    _buttonsInAutoRepeatMode.insert(buttonId);
    _isInAutoRepeatMode[buttonId] = false;
    _autoRepeatNotified[buttonId] = false; // Reset notification status
  }
}

void AutoRepeatHandler::handleButtonReleased(const std::string &buttonId) {
  _buttonsInAutoRepeatMode.erase(buttonId);
  _isInAutoRepeatMode[buttonId] = false;
  _autoRepeatNotified[buttonId] = false;
}

void AutoRepeatHandler::checkAutoRepeat() {
  auto now = std::chrono::steady_clock::now();
  for (const auto &buttonId : _buttonsInAutoRepeatMode) {
    auto &autoRepeatSettings = _cachedAutoRepeatSettings[buttonId];
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - _lastPressTime[buttonId]);

    // Check if the button is eligible for auto-repeat mode
    if (!_isInAutoRepeatMode[buttonId] &&
        elapsed.count() >= autoRepeatSettings.initialDelayMs) {
      _isInAutoRepeatMode[buttonId] = true;
      _autoRepeatNotified[buttonId] = false; // Ensure it's ready to notify
      _lastPressTime[buttonId] =
          now; // Reset the timer for auto-repeat rate calculation
    }

    // Check if it's time to send an auto-repeat notification
    if (_isInAutoRepeatMode[buttonId]) {
      auto elapsedSinceModeStarted =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              now - _lastPressTime[buttonId]);
      if (elapsedSinceModeStarted.count() >=
          autoRepeatSettings.standardRateMs) {
        if (!_autoRepeatNotified[buttonId]) {
          _controller->notify("button_auto_repeat", buttonId);
          _autoRepeatNotified[buttonId] = true; // Mark as notified
        }
        _lastPressTime[buttonId] = now; // Reset the timer for the next interval
      }
    }
  }
}
