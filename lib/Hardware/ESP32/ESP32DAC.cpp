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

void ESP32DAC::applyValue(int value) {
  // Apply the value to the DAC, ensuring it's within the valid range
  int clampedValue = std::max(0, std::min(value, 255));
  dacWrite(_pinNumber, clampedValue);
  Logger::debug("[ESP32DAC] DAC applyValue: Pin " + std::to_string(_pinNumber) +
                ", Value " + std::to_string(clampedValue));
}

#endif // PLATFORM_ESP32
