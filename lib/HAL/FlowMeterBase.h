// FlowMeterBase.h
#pragma once

#include "IFlowMeter.h"

class FlowMeterBase : public IFlowMeter {
public:
  explicit FlowMeterBase(const HardwarePinConfig &config)
      : IFlowMeter(config), totalVolume(0.0), flowRate(0.0) {}

  virtual ~FlowMeterBase() override = default;

  virtual double getFlowRate() const override {
    // Implement flow rate calculation
    return flowRate;
  }

  virtual double getTotalVolume() const override {
    // Implement total volume calculation
    return totalVolume;
  }

  virtual void reset() override { totalVolume = 0.0; }

protected:
  double totalVolume; // Total volume that has passed through the flow meter
  double flowRate;    // Current flow rate
};
