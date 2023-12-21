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
        _isInAutoRepeatMode(false),
        _wasPressed(false) {}

  virtual bool isPressed() const override { return _isPressed; }

  virtual void setOnPressCallback(ButtonPressCallback callback) override {
    _onPressCallback = callback;
  }

  virtual void setAutoRepeatSettings(
      const InteractionSettings::AutoRepeat &settings) override {
    _autoRepeatSettings = settings;
  }

  virtual void update() override {
    updateButtonState();  // Reflect hardware state in _isPressed

    // Handle state change: press or release
    if (_isPressed != _wasPressed) {
      handleStateChange();
    }

    // Auto-repeat functionality for held-down button
    if (_isPressed && shouldAutoRepeat()) {
      invokePressCallback();
    }
  }

  // Method to get the current state of the button
  ButtonState getCurrentState() const {
    ButtonState state;
    state.isPressed = _isPressed;
    state.isInAutoRepeatMode = _isInAutoRepeatMode;

    return state;
  }

 protected:
  // Must be implemented in derived classes to update the _isPressed state
  virtual void updateButtonState() = 0;

  bool _isPressed;   // Current pressed state of the Button
  bool _wasPressed;  // Tracks the previous pressed state
  ButtonPressCallback _onPressCallback = nullptr;  // Callback for button press
  std::chrono::steady_clock::time_point _lastPressTime;
  InteractionSettings::AutoRepeat _autoRepeatSettings;  // Auto-repeat settings
  bool _isInAutoRepeatMode;  // Flag to indicate if the button is in auto-repeat
                             // mode

  // Testing utility: Adjusts the last press time for simulating time passage
  virtual void adjustLastPressTimeForTesting(
      std::chrono::milliseconds duration) {
    _lastPressTime -= duration;
  }

 private:
  void invokePressCallback() {
    if (_onPressCallback) {
      _onPressCallback(id());
    }
  }

  void handleStateChange() {
    resetLastPressTime();         // Resetting time on state change
    _isInAutoRepeatMode = false;  // Reset auto-repeat mode on any state change

    if (_isPressed) {
      // Optionally, invoke the callback only on press
      invokePressCallback();
    }

    _wasPressed = _isPressed;
  }

  void resetLastPressTime() {
    _lastPressTime = std::chrono::steady_clock::now();
  }

  void enterAutoRepeatMode() {
    _isInAutoRepeatMode = true;
    resetLastPressTime();  // Reset time when entering auto-repeat mode
  }

  bool shouldAutoRepeat() {
    auto elapsedTime = getElapsedTime();

    if (!_isInAutoRepeatMode &&
        elapsedTime >= _autoRepeatSettings.initialDelayMs) {
      enterAutoRepeatMode();
      return true;
    }

    if (_isInAutoRepeatMode &&
        elapsedTime >= _autoRepeatSettings.standardRateMs) {
      resetLastPressTime();  // Reset for next interval
      return true;
    }

    return false;
  }

  uint32_t getElapsedTime() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsedDuration =
        std::chrono::duration_cast<std::chrono::milliseconds>(now -
                                                              _lastPressTime);
    return static_cast<uint32_t>(elapsedDuration.count());
  }
};
