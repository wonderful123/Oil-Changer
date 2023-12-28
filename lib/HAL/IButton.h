#pragma once

#include "HardwareComponent.h"
#include <functional>
#include <string>

class IButton : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor

  virtual ~IButton() = default; // Virtual destructor for cleanup

  struct ButtonState {
    bool isPressed;
  };

  virtual ButtonState getCurrentState() const = 0;
  virtual ButtonState getPreviousState() const = 0;

  virtual bool isPressed() const = 0; // Check if the button is pressed

  virtual void update() = 0; // Update the button's state
};
