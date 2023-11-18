#pragma once

#include "GpioPinConfig.h"
#include "HardwareComponent.h"

class IBuzzer : public HardwareComponent {
public:
  virtual ~IBuzzer() = default;
  virtual void beep(int frequency, int duration) = 0;
  virtual void stop() = 0;
  virtual void update() = 0;
  virtual void setVolume(float volume) = 0;

protected:
  explicit IBuzzer(const GpioPinConfig &config) : HardwareComponent(config) {}
};
