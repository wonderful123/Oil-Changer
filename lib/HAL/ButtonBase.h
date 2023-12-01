#pragma once

#include "IButton.h"
#include <chrono>

class ButtonBase : public IButton {
public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config), _isPressed(false), _lastPressTime(),
        _autoRepeatSettings() {}

  virtual bool isPressed() const override { return _isPressed; }

  void setOnPressCallback(ButtonPressCallback callback) override {
    _onPressCallback = callback;
  }

  virtual void update() = 0; // Must be implemented in derived classes

  // Implement setAutoRepeatSettings
  void setAutoRepeatSettings(
      const InteractionSettings::AutoRepeat &settings) override {
    _autoRepeatSettings = settings;
  }

protected:
  bool _isPressed; // Current pressed state of the Button
  ButtonPressCallback _onPressCallback = nullptr; // Callback for button press

  // Store last press time for auto-repeat logic
  std::chrono::steady_clock::time_point _lastPressTime;
  InteractionSettings::AutoRepeat _autoRepeatSettings; // Auto-repeat settings
};
