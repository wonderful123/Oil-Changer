#pragma once

#include "DigitalIOBase.h"

class ESP32DigitalIO : public DigitalIOBase {
public:
  ESP32DigitalIO(const HardwarePinConfig &config);
  virtual ~ESP32DigitalIO() = default;
  virtual int read() const override;
  virtual void write(int value) override;
};
