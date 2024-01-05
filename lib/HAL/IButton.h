#pragma once

#include "HardwareComponent.h"
#include <functional>
#include <string>

class IButton : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor

  virtual ~IButton() = default; // Virtual destructor for cleanup

  virtual void updateButton() = 0; // Update the button's press/release
                                      // state through the hardware

  enum class State { Released, Pressed, Held, HeldLong };

  virtual std::string stateToString(State state) const = 0;

  virtual void setHoldDurationThreshold(int duration) = 0;
  virtual void setLongHoldDurationThreshold(int duration) = 0;

  virtual State getState() const = 0;

  virtual void updateButtonState() = 0;

  virtual const std::string &idString() const = 0;
};
