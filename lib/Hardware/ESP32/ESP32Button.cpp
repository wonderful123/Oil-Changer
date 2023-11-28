#include "ESP32Button.h"
#include "Logger.h"
#include <Arduino.h>

ESP32Button::ESP32Button(const HardwarePinConfig &config) : ButtonBase(config) {
  _debouncer.attach(_pinNumber, INPUT_PULLUP);
  _debouncer.interval(50); // Debounce interval
  Logger::info("Button: " + id() + " initialized.");
}

void ESP32Button::update() {
  _debouncer.update();
  if (_debouncer.fell()) {
    _isPressed = true;
    if (_onPressCallback) {
      _onPressCallback(_id);
    }
  } else if (_debouncer.rose()) {
    _isPressed = false;
  }
}
