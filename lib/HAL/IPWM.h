#pragma once

#include "HardwareComponent.h"
#include "HardwarePinConfig.h"

class IPWM : public HardwareComponent {
public:
  using HardwareComponent::HardwareComponent; // Inherit constructor
  
  virtual ~IPWM() = default;
  virtual void setDutyCycle(double dutyCycle) = 0;
  virtual double getDutyCycle() const = 0;
  virtual void setFrequency(double frequency) = 0;
  virtual double getFrequency() const = 0;
};
