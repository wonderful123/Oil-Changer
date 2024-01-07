// ESP32DAC.cpp

#ifdef PLATFORM_ESP32

#include "ESP32DAC.h"
#include "Logger.h"

ESP32DAC::ESP32DAC(const HardwarePinConfig &config) : DACBase(config) {
  // Check if the pin is a valid DAC pin
  if (_pinNumber == 25 || _pinNumber == 26) {
    setInitialized(true); // Mark the DAC as initialized
  } else {
    Logger::error("[ESP32DAC] Invalid DAC pin: " + std::to_string(_pinNumber));
    setInitialized(false); // Mark the DAC as not initialized due to invalid pin
  }
}

// value is between 0 and 1
void ESP32DAC::applyValue(float value) {
  // Convert the value to a 8-bit unsigned integer between 0 and 255
  uint8_t convertedValue = static_cast<uint8_t>(value * 255);
  dacWrite(_pinNumber, convertedValue);
  Logger::debug("[ESP32DAC] DAC applyValue: Pin " + std::to_string(_pinNumber) +
                ", Value " + std::to_string(convertedValue));
}

#endif // PLATFORM_ESP32
