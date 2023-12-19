#pragma once

#include <chrono>

#include "IButton.h"

class ButtonBase : public IButton {
 public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config),
        _isPressed(false),
        _lastPressTime(std::chrono::steady_clock::now()),
        _autoRepeatSettings(),
        _isInAutoRepeatMode(false) {}

  virtual bool isPressed() const override { return _isPressed; }

  void setOnPressCallback(ButtonPressCallback callback) override {
    _onPressCallback = callback;
  }

  virtual void update() = 0;  // Must be implemented in derived classes

  void setAutoRepeatSettings(
      const InteractionSettings::AutoRepeat &settings) override {
    _autoRepeatSettings = settings;
  }

  // Utility method to check for auto-repeat
  bool checkAutoRepeat() {
    auto now = std::chrono::steady_clock::now();
    if (_isPressed) {
      auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                         now - _lastPressTime)
                         .count();
      if (!_isInAutoRepeatMode &&
          elapsed >= _autoRepeatSettings.initialDelayMs) {
        _isInAutoRepeatMode = true;
        _lastPressTime = now;
        return true;
      } else if (_isInAutoRepeatMode &&
                 elapsed >= _autoRepeatSettings.standardRateMs) {
        _lastPressTime = now;
        return true;
      }
    } else {
      _isInAutoRepeatMode = false;
    }
    return false;
  }

  // Method to get the current state of the button
  ButtonState getCurrentState() const {
    ButtonState state;
    state.isPressed = _isPressed;
    state.isInAutoRepeatMode = _isInAutoRepeatMode;

    return state;
  }

 protected:
  bool _isPressed;  // Current pressed state of the Button
  ButtonPressCallback _onPressCallback = nullptr;  // Callback for button press
  std::chrono::steady_clock::time_point _lastPressTime;
  InteractionSettings::AutoRepeat _autoRepeatSettings;  // Auto-repeat settings
  bool _isInAutoRepeatMode;  // Flag to indicate if the button is in auto-repeat
                             // mode
};
