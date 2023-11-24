#pragma once

#ifdef PLATFORM_ESP32

#include "HardwarePinConfig.h"
#include <cstddef> // For size_t

class ISPI {
public:
  virtual ~ISPI() = default;

  virtual void begin() = 0;
  virtual void end() = 0;
  virtual uint8_t transfer(uint8_t data) = 0;
  virtual void transfer(void *buf, size_t count) = 0;
};

#endif // PLATFORM_ESP32