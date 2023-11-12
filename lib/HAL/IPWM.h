#pragma once

#include <ComponentConfig.h>

class IPWM {
public:
  virtual ~IPWM() = default;

  virtual void setDutyCycle(double dutyCycle) = 0;
  virtual double getDutyCycle() const = 0;
  virtual void setFrequency(double frequency) = 0;
  virtual double getFrequency() const = 0;
};
