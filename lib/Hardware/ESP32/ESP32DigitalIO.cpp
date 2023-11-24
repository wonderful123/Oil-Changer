#ifdef PLATFORM_ESP32

#include "ESP32DigitalIO.h"
#include "Error.h"
#include <Arduino.h>

ESP32DigitalIO::ESP32DigitalIO(const HardwarePinConfig &config)
    : DigitalIOBase(config) {
  pinMode(config.pinNumber, _mode);
}

int ESP32DigitalIO::read() const { return digitalRead(_pinNumber); }

void ESP32DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

#endif // PLATFORM_ESP32
