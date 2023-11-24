#pragma once

#include "HardwarePinConfig.h"
#include "HardwareComponent.h"
#include "IADC.h"
#include "Logger.h"

class ADCBase : public IADC {
public:
  explicit ADCBase(const HardwarePinConfig &config)
      : IADC(config) { // Default resolution
    extractResolution(config);
  }

  virtual int resolution() const override { return _resolution; }

protected:
  void extractResolution(const HardwarePinConfig &config) {
    int res = config.getOptionAs<int>("resolution");
    if (res > 0) {
      _resolution = res;
      Logger::info("ADC resolution set to " + std::to_string(_resolution));
    } else {
      Logger::warn("ADC resolution not specified. Using default: " +
                   std::to_string(_resolution));
    }
  }
};
