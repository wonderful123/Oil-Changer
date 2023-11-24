#pragma once

#include "IButton.h"

/**
 * @class ButtonBase
 * @brief Base class for Button implementations.
 *
 * Provides common functionality for Button components.
 */
class ButtonBase : public IButton {
public:
  explicit ButtonBase(const HardwarePinConfig &config)
      : IButton(config), _isPressed(false) {}

  // Check if the button is currently pressed
  virtual bool isPressed() const override { return _isPressed; }

  void setOnPressCallback(ButtonPressCallback callback) override {
    _onPressCallback = callback;
  }

protected:
  /**
   * @brief Update the pressed state of the button.
   *
   * This method should be implemented in derived classes to
   * update the button's pressed state.
   */
  virtual void updatePressedState() = 0;

  ButtonPressCallback _onPressCallback;

private:
  bool _isPressed; // Current pressed state of the Button
};
