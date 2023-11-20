#pragma once

#include "DigitalIOBase.h"

class DigitalIO : public DigitalIOBase {
public:
  virtual ~DigitalIO() = default;
  virtual int read() const override;
  virtual void write(int value) override;
};
