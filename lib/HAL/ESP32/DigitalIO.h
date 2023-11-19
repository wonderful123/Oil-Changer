#pragma once

#include "IDigitalIO.h"

class DigitalIO : public IDigitalIO {
public:
  DigitalIO(const GpioPinConfig &config);
  virtual ~DigitalIO() = default;
  virtual int read() const override;
  virtual void write(int value) override;
  virtual Mode getMode() const override;

private:
  void setPinModeFromConfig(const GpioPinConfig &config) const;
};
