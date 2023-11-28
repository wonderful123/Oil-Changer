#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"
#include <functional>

class IButton : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor

  virtual ~IButton() = default; // Virtual destructor for cleanup

  virtual bool isPressed() const = 0; // Check if the button is pressed

  using ButtonPressCallback = void (*)(const std::string &);
  virtual void setOnPressCallback(ButtonPressCallback callback) = 0;

  virtual void update() = 0; // Update the button's state
};
