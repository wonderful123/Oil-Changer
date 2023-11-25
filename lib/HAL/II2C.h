#pragma once

#include "HardwarePinConfig.h"
#include <cstddef>
#include <cstdint>

class II2C {
public:
  virtual ~II2C() = default;

  virtual void begin() = 0;
  virtual void end() = 0;
  virtual void beginTransmission(uint8_t address) = 0;
  virtual uint8_t endTransmission(bool sendStop = true) = 0;
  virtual size_t write(uint8_t value) = 0;
  virtual int read() = 0;
  virtual int available() = 0;
};