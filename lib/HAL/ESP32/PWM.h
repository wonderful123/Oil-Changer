#pragma once

#include "IPWM.h"
#include <Arduino.h>

class PWM : public IPWM {
public:
  PWM(int pin, const ComponentConfig &config);
  virtual ~PWM();

  void setDutyCycle(double dutyCycle) override;
  double getDutyCycle() const override;

  void setFrequency(double frequency) override;
  double getFrequency() const override;

private:
  int _pinNumber;           // The GPIO pin number for the PWM signal
  double _frequency;        // Frequency of the PWM signal
  double _dutyCycle;        // Duty cycle of the PWM signal
  ConfigMap _configOptions; // Additional configuration options for PWM
};