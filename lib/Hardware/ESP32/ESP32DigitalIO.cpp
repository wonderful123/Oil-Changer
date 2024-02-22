// ESP32DigitalIO.cpp

#ifdef PLATFORM_ESP32

#include "ESP32DigitalIO.h"
#include "Error.h"
#include <Arduino.h>

ESP32DigitalIO::ESP32DigitalIO(const HardwarePinConfig &config)
    : DigitalIOBase(config) {
  _pinNumber = config.pinNumber;
  setMode(_mode);
  setInitialized(true);
}

int ESP32DigitalIO::read() const { return digitalRead(_pinNumber); }

void ESP32DigitalIO::write(int value) { digitalWrite(_pinNumber, value); }

void ESP32DigitalIO::setMode(Mode mode) {
  _mode = mode;
  uint8_t espMode = INPUT; // Default to INPUT (Arduino's macro)
  switch (mode) {
  case Mode::MODE_INPUT:
    espMode = INPUT;
    break;
  case Mode::MODE_OUTPUT:
    espMode = OUTPUT;
    break;
  case Mode::MODE_PULLUP:
    espMode = INPUT_PULLUP;
    break;
  case Mode::MODE_PULLDOWN:
    espMode = INPUT_PULLDOWN;
    break;
  }
  pinMode(_pinNumber, espMode);
}

#endif // PLATFORM_ESP32
