// ESP32Serial.h
#pragma once

#ifdef PLATFORM_ESP32

#include <HardwareSerial.h>

#include "ISerial.h"

class HardwarePinConfig;  // Ensure this is defined appropriately

class ESP32Serial : public ISerial {
 public:
   explicit ESP32Serial(const HardwarePinConfig &config);

   void begin() override;
   void end() override;
   void write(const std::string &message) override;
   int read() override;
   int available() override;
   void flush() override;

 private:
  HardwareSerial _serial;
  int _rxPin, _txPin, _uartPort;
  unsigned long _baudRate;

  // Utility function to determine the UART port based on the configuration
  static int determineUartPort(const HardwarePinConfig &config);
  // Utility to initialize the serial communication
  void initializeSerial();
};

#endif  // PLATFORM_ESP32
