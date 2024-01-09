// FlowMeterBase.h
#pragma once

#include "IFlowMeter.h"

class FlowMeterBase : public IFlowMeter {
public:
  explicit FlowMeterBase(const HardwarePinConfig &config)
      : IFlowMeter(config), _totalVolume(0.0), _flowRate(0.0) {
    _pulsesPerLiter = config.getOptionAs<double>("pulsesPerLiter");
  }

  virtual ~FlowMeterBase() override = default;

  // Set calibration factor (pulses per liter)
  virtual void setCalibration(double pulsesPerLiter) override {
    _pulsesPerLiter = pulsesPerLiter;
  }

  virtual double getFlowRate() const override {
    return _flowRate;
  }

  virtual double getTotalVolume() const override {
    // Implement total volume calculation
    return _totalVolume;
  }

  virtual void reset() override { _totalVolume = 0.0; }

protected:
  double _totalVolume;    // Total volume that has passed through the flow meter
  double _flowRate;       // Current flow rate
  double _pulsesPerLiter; // Pulses per unit volume (e.g., per liter)
};
