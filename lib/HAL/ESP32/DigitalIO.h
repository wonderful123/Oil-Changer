#pragma once

#include "GpioPinConfig.h"
#include "HardwareComponent.h"

class IDigitalIO : public HardwareComponent {
public:
  enum Mode { INPUT, OUTPUT };

  virtual ~IDigitalIO() = default;
  virtual int read() const = 0;
  virtual void write(int value) = 0;
  virtual Mode getMode() const = 0;

protected:
  explicit IDigitalIO(const GpioPinConfig &config)
      : HardwareComponent(config) {}
};
