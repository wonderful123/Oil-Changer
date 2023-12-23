#pragma once

#include <string>

#include "HardwareComponent.h"

class HardwarePinConfig;

class ISerial : public HardwareComponent {
 public:
  explicit ISerial(const HardwarePinConfig &config)
      : HardwareComponent(config) {}

  virtual ~ISerial() = default;

  virtual void begin(unsigned long baudrate) = 0;
  virtual void end() = 0;
  virtual void write(const std::string &message) = 0;
  virtual int read() = 0;
  virtual int available() = 0;
  virtual void flush() = 0;

 protected:
  int _baud = 115200;
};
