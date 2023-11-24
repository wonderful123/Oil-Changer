#pragma once

#include "HardwarePinConfig.h"
#include "HardwareComponent.h"
#include "Logger.h"

class IBuzzer : public HardwareComponent {
public:
  virtual ~IBuzzer() = default;
  virtual void beep(int frequency, int duration) = 0;
  virtual void stop() = 0;
  virtual bool isBeeping() const = 0;
  virtual void setVolume(float volume) = 0;

protected:
  explicit IBuzzer(const HardwarePinConfig &config) : HardwareComponent(config) {}
};
