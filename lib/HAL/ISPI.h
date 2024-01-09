// ISPI.h
#pragma once

#include <cstddef> // For size_t
#include <cstdint> // For uint8_t

#include "HardwareComponent.h"
#include "ICommunicationInterface.h"

class HardwarePinConfig;

class ISPI : public ICommunicationInterface {
public:
  explicit ISPI(const HardwarePinConfig &config)
      : ICommunicationInterface(config) {}

  virtual ~ISPI() = default;

  virtual void begin() = 0;
  virtual void end() = 0;
  virtual uint8_t transfer(uint8_t data) = 0;
  virtual void transfer(void *buf, size_t count) = 0;
};
