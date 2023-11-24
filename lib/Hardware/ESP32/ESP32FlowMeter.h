#pragma once

#ifdef PLATFORM_ESP32

#include "FlowMeterBase.h"
#include "driver/pcnt.h"
#include <functional>

class ESP32FlowMeter : public FlowMeterBase {
public:
  using PulseCallback = std::function<void()>;

  ESP32FlowMeter(const HardwarePinConfig &config);
  virtual ~ESP32FlowMeter() override;

  // Overrides from FlowMeterBase
  virtual double getFlowRate() const override;
  virtual void reset() override;

protected:
  // Method to initialize PCNT for the given pin
  void initPcnt(pcnt_unit_t pcntUnit);

private:
  volatile int16_t _pulseCount;
  PulseCallback _pulseCallback;
  pcnt_unit_t _pcntUnit;

  // Helper function to get the current pulse count
  int16_t getPulseCount() const;
};

#endif // PLATFORM_ESP32