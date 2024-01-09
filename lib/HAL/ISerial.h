// ISerial.h
#pragma once

#include <string>

#include "HardwareComponent.h"
#include "ICommunicationInterface.h"

class ISerial : public ICommunicationInterface {
public:
  explicit ISerial(const HardwarePinConfig &config)
      : ICommunicationInterface(config) {}

  virtual ~ISerial() = default;

  virtual void begin() = 0;
  virtual void end() = 0;
  virtual void write(const std::string &message) = 0;
  virtual int read() = 0;
  virtual int available() = 0;
  virtual void flush() = 0;

protected:
  int _baud = 115200;
};
