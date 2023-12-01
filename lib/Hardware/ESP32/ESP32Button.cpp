#ifdef PLATFORM_ESP32

#include "ESP32Button.h"
#include "Logger.h"
#include <Arduino.h>

ESP32Button::ESP32Button(const HardwarePinConfig &config) : ButtonBase(config) {
  pinMode(config.pinNumber, INPUT_PULLUP);
  _debouncer.attach(config.pinNumber);
  _debouncer.interval(50); // Debounce interval
  Logger::info("Button: " + id() + " initialized.");
}

void ESP32Button::update() {
  _debouncer.update();
  bool previouslyPressed = _isPressed;
  _isPressed = _debouncer.read() == LOW; // Assuming active LOW

  if (_isPressed && !previouslyPressed) {
    _lastPressTime = std::chrono::steady_clock::now();
    if (_onPressCallback) {
      _onPressCallback(id());
    }
  } else if (!_isPressed && previouslyPressed) {
    _isInAutoRepeatMode = false;
  }

  if (checkAutoRepeat() && _onPressCallback) {
    _onPressCallback(id());
  }
}

#endif // PLATFORM_ESP32
