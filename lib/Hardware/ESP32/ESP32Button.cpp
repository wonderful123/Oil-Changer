#ifdef PLATFORM_ESP32

#include "ESP32Button.h"
#include "Logger.h"
#include <Arduino.h>

ESP32Button::ESP32Button(const HardwarePinConfig &config) : ButtonBase(config) {
  pinMode(config.pinNumber, INPUT_PULLUP);
  _debouncer.attach(config.pinNumber);
  _debouncer.interval(50); // Debounce interval
}

void ESP32Button::updateButtonState() {
  _debouncer.update();
  _isPressed = _debouncer.read() == LOW;  // Assuming active LOW
}

#endif // PLATFORM_ESP32
