#pragma once

#include "IPWM.h"
#include "Logger.h"

class PWMBase : public IPWM {
 public:
  explicit PWMBase(const HardwarePinConfig &config)
      : IPWM(config), _dutyCycle(0.0), _frequency(1000.0) {  // Default values
    extractFrequency(config);
  }

  virtual void setDutyCycle(double dutyCycle) override {
    _dutyCycle =
        std::max(0.0, std::min(dutyCycle, 1.0));  // Clamp between 0.0 and 1.0
    applyDutyCycle(_dutyCycle);
  }

  virtual double getDutyCycle() const override { return _dutyCycle; }

  virtual void setFrequency(double frequency) override {
    _frequency = frequency;
    applyFrequency(_frequency);
  }

  virtual double getFrequency() const override { return _frequency; }

 protected:
  void extractFrequency(const HardwarePinConfig &config) {
    double freq = config.getOptionAs<double>("frequency");
    if (freq > 0.0) {
      _frequency = freq;
      Logger::info("PWM frequency set to " + std::to_string(_frequency));
    } else {
      Logger::warn("PWM frequency not specified. Using default: " +
                   std::to_string(_frequency));
    }
  }

  virtual void applyDutyCycle(
      double dutyCycle) = 0;  // Platform-specific implementation
  virtual void applyFrequency(
      double frequency) = 0;  // Platform-specific implementation

 private:
  double _dutyCycle;
  double _frequency;
};
