#pragma once

#ifdef PLATFORM_NATIVE

#include "IADC.h"
#include <iostream>
#include <unordered_map>

class MockADC : public IADC {
public:
  explicit MockADC(const HardwarePinConfig &config)
      : IADC(config), _mockValue(0), _resolution(12) {
    // Simulate setting resolution and other properties from config
    auto resIt = config.options.find("resolution");
    if (resIt != config.options.end()) {
      _resolution = std::stoi(resIt->second);
    }
    // Simulate successful initialization
    setInitialized(true);
    std::cout << "MockADC initialized with resolution: " << _resolution
              << std::endl;
  }

  virtual int read() const override {
    if (!isInitialized()) {
      std::cerr << "MockADC: Attempted to read uninitialized ADC." << std::endl;
      return -1; // or some error code
    }
    // Return a simulated ADC value
    return _mockValue;
  }

  virtual int resolution() const override { return _resolution; }

  // Additional methods for testing
  void setMockValue(int value) { _mockValue = value; }

private:
  int _mockValue;  // Simulated ADC value
  int _resolution; // Resolution of the ADC
};

#endif // PLATFORM_NATIVE