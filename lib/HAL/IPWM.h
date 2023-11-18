#pragma once

#include "GpioPinConfig.h"
#include "HardwareComponent.h"

class IPWM : public HardwareComponent {
public:
  virtual ~IPWM() = default;
  virtual void setDutyCycle(double dutyCycle) = 0;
  virtual double getDutyCycle() const = 0;
  virtual void setFrequency(double frequency) = 0;
  virtual double getFrequency() const = 0;

protected:
  explicit IPWM(const GpioPinConfig &config) : HardwareComponent(config) {}
};
