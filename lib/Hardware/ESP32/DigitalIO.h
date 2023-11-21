#pragma once

#include "DigitalIOBase.h"

class DigitalIO : public DigitalIOBase {
public:
  DigitalIO(const GpioPinConfig &config);
  virtual ~DigitalIO() = default;
  virtual int read() const override;
  virtual void write(int value) override;
};
