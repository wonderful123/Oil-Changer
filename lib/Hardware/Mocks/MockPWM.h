#pragma once

#include "IPWM.h"

#ifdef PLATFORM_NATIVE

#include <iostream>

class MockPWM : public IPWM {
public:
    explicit MockPWM(const HardwarePinConfig& config) : IPWM(config), _dutyCycle(0.0), _frequency(1000.0) {
        // Simulate setting frequency from config if available
        auto frequencyIter = config.options.find("frequency");
        if (frequencyIter != config.options.end()) {
            _frequency = std::stod(frequencyIter->second);
        }
        std::cout << "MockPWM initialized on pin: " << _pinNumber << " with frequency: " << _frequency << std::endl;
        setInitialized(true); // Mark MockPWM as initialized
    }

    void setDutyCycle(double dutyCycle) override {
      if (dutyCycle < 0.0)
        _dutyCycle = 0.0;
      if (dutyCycle > 1.0)
        _dutyCycle = 1.0;
      std::cout << "MockPWM: Duty cycle set to " << _dutyCycle << std::endl;
    }

    double getDutyCycle() const override {
        return _dutyCycle;
    }

    void setFrequency(double frequency) override {
        _frequency = frequency;
        std::cout << "MockPWM: Frequency set to " << _frequency << std::endl;
    }

    double getFrequency() const override {
        return _frequency;
    }

private:
    double _frequency; // Simulated frequency of the PWM signal
    double _dutyCycle; // Simulated duty cycle of the PWM signal
};

#endif