// II2C.h
#pragma once

#include <cstddef>
#include <cstdint>

#include "HardwareComponent.h"
#include "ICommunicationInterface.h"

class HardwarePinConfig;

class II2C : public ICommunicationInterface {
public:
  explicit II2C(const HardwarePinConfig &config)
      : ICommunicationInterface(config) {}

  virtual ~II2C() = default;

  virtual void begin() = 0;
  virtual void end() = 0;
  virtual void beginTransmission(uint8_t address) = 0;
  virtual uint8_t endTransmission(bool sendStop = true) = 0;
  virtual size_t write(uint8_t value) = 0;
  virtual int read() = 0;
  virtual int available() = 0;
};