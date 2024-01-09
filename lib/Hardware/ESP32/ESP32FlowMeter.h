// ESP32FlowMeter.h
#pragma once

#ifdef PLATFORM_ESP32

#include <driver/pcnt.h>

#include "Error.h"
#include "FlowMeterBase.h"
#include <driver/pcnt.h>
#include <functional>

class ESP32FlowMeter : public FlowMeterBase {
public:
  ESP32FlowMeter(const HardwarePinConfig &config);
  virtual ~ESP32FlowMeter() override;

  void update() override;
  virtual void reset() override;

private:
  pcnt_unit_t _pcntUnit;

  // Minimum time between updates of flowrate and volume calculation
  int _minUpdateIntervalMs;
  unsigned long _lastUpdateTime;

  // Method to initialize PCNT for the given pin
  Error initPcnt(const HardwarePinConfig &config);

  void resetPcntCounter();

  // Helper function to get the current pulse count
  int16_t getPulseCount() const;
};

#endif // PLATFORM_ESP32