#ifdef PLATFORM_ESP32

#include "ESP32Button.h"

#include <Arduino.h>

#include "Logger.h"

ESP32Button::ESP32Button(const HardwarePinConfig &config,
                         unsigned long debounceDuration)
    : ButtonBase(config, debounceDuration) {
  _debouncer = Bounce();
  _debouncer.interval(debounceDuration);
  _debouncer.attach(config.pinNumber);
}

void ESP32Button::updateButtonState() {
  _debouncer.update();
  if (_debouncer.changed()) {
    _isPressed = !_debouncer.read();
  }
}

#endif // PLATFORM_ESP32
