#pragma once

#include <string>

class ISerial {
public:
  virtual ~ISerial() = default;

  virtual void begin(unsigned long baudrate) = 0;
  virtual void end() = 0;
  virtual size_t write(uint8_t byte) = 0;
  virtual int read() = 0;
  virtual int available() = 0;
  virtual void flush() = 0;
};
