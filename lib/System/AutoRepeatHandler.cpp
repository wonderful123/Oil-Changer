// AutoRepeatHandler.cpp
#include "AutoRepeatHandler.h"
#include "Logger.h"

AutoRepeatHandler::AutoRepeatHandler(
    std::shared_ptr<ButtonController> controller,
    const InteractionSettings &settings)
    : _controller(controller), _settings(settings) {}

void AutoRepeatHandler::onNotify(const std::string &event,
                                 const std::string &id) {
  if (event == "button_pressed") {
    handleButtonPressed(id);
  } else if (event == "button_released") {
    handleButtonReleased(id);
  }
}

void AutoRepeatHandler::handleButtonPressed(const std::string &id) {
  _lastPressTime[id] = std::chrono::steady_clock::now();
  // Check if we should consider auto-repeat for this button
  if (_settings.buttons.find(id) != _settings.buttons.end() &&
      _settings.buttons[id].hasAutoRepeat) {
    checkAutoRepeat(id);
  }
}

void AutoRepeatHandler::handleButtonReleased(const std::string &id) {
  _lastPressTime.erase(id);
  _isInAutoRepeatMode[id] = false;
}

void AutoRepeatHandler::checkAutoRepeat(const std::string &id) {
  if (_lastPressTime.find(id) != _lastPressTime.end()) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - _lastPressTime[id]);

    auto &autoRepeatSettings = _settings.buttons[id].autoRepeat;

    if (!_isInAutoRepeatMode[id] &&
        elapsed.count() >= autoRepeatSettings.initialDelayMs) {
      _isInAutoRepeatMode[id] = true;
      _controller->notify("button_auto_repeat", id);
      _lastPressTime[id] = now; // Reset the timer for the next interval
    } else if (_isInAutoRepeatMode[id] &&
               elapsed.count() >= autoRepeatSettings.standardRateMs) {
      _controller->notify("button_auto_repeat", id);
      _lastPressTime[id] = now; // Reset the timer for the next interval
    }
  }
}
