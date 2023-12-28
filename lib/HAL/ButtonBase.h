#pragma once

#include "IButton.h"
#include <chrono>

class ButtonBase : public IButton {
public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config), _isPressed(false), _wasPressed(false) {}

  virtual bool isPressed() const override { return _isPressed; }

  virtual void update() override {
    _wasPressed = _isPressed; // Store the previous state
    updateButtonState();      // Reflect hardware state in _isPressed

    if (_isPressed != _wasPressed) {
      _lastPressTime = std::chrono::steady_clock::now();
    }
  }

  // Method to get the current state of the button
  IButton::ButtonState getCurrentState() const override {
    return IButton::ButtonState{_isPressed};
  }

  // Method to get the previous state of the button
  IButton::ButtonState getPreviousState() const override {
    return IButton::ButtonState{_wasPressed};
  }

protected:
  // Must be implemented in derived classes to update the _isPressed state
  virtual void updateButtonState() = 0;

  bool _isPressed;  // Current pressed state of the Button
  bool _wasPressed; // Previous pressed state of the Button
  std::chrono::steady_clock::time_point
      _lastPressTime; // Time of the last state change
};
