#pragma once

#include "GpioPinConfig.h"
#include "IPWM.h"
#include <Arduino.h>

class PWM : public IPWM {
public:
  /**
   * @brief Construct a new PWM object for ESP32.
   *
   * @param config Configuration for the PWM pin.
   */
  PWM(const GpioPinConfig &config);
  virtual ~PWM();

  void setDutyCycle(double dutyCycle) override;
  double getDutyCycle() const override;

  void setFrequency(double frequency) override;
  double getFrequency() const override;

private:
  int _pinNumber;    // The GPIO pin number for the PWM signal
  double _frequency; // Frequency of the PWM signal
  double _dutyCycle; // Duty cycle of the PWM signal
};
