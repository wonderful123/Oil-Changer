// ESP32Button.cpp

#ifdef PLATFORM_ESP32

#include "ESP32Button.h"
#include <Arduino.h>

ESP32Button::ESP32Button(const HardwarePinConfig &config) : ButtonBase(config) {
  pinMode(config.pinNumber, INPUT_PULLUP);
  _debouncer = Bounce();
  _debouncer.interval(50); // Debounce interval
  _debouncer.attach(config.pinNumber);
}

void ESP32Button::updateButton() {
  _debouncer.update();
  if (_debouncer.changed()) {
    _isPressed = !_debouncer.read();
  }
}

void ESP32Button::setDebounceDuration(int durationMs) {
  _debouncer.interval(durationMs);
}

#endif // PLATFORM_ESP32
