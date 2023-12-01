#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"
#include "InteractionSettings.h"
#include <functional>
#include <string>

class IButton : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor

  virtual ~IButton() = default; // Virtual destructor for cleanup

  virtual bool isPressed() const = 0; // Check if the button is pressed

  using ButtonPressCallback = std::function<void(const std::string &)>;
  virtual void setOnPressCallback(ButtonPressCallback callback) = 0;

  virtual void update() = 0; // Update the button's state

  // Add method to set auto-repeat settings
  virtual void
  setAutoRepeatSettings(const InteractionSettings::AutoRepeat &settings) = 0;
};
