#pragma once

#include "IButton.h"

class ButtonBase : public IButton {
public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config), _isPressed(false) {}

  virtual bool isPressed() const override { return _isPressed; }

  void setOnPressCallback(ButtonPressCallback callback) override {
    _onPressCallback = callback;
  }

  virtual void update() = 0; // Must be implemented in derived classes

protected:
  bool _isPressed; // Current pressed state of the Button
  ButtonPressCallback _onPressCallback = nullptr; // Callback for button press
};
