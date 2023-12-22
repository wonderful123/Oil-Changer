#pragma once

#include "HardwareComponent.h"

class IFlowMeter : public HardwareComponent {
 public:
  using HardwareComponent::HardwareComponent;  // Inherit constructor

  virtual ~IFlowMeter() = default;

  // Returns the current flow rate
  virtual double getFlowRate() const = 0;

  // Returns the total volume passed through the flow meter
  virtual double getTotalVolume() const = 0;

  // Resets the flow meter readings
  virtual void reset() = 0;
};
