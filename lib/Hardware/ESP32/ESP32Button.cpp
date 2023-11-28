#ifdef PLATFORM_ESP32

#include "ESP32Button.h"
#include "Logger.h"

std::unordered_map<int, ESP32Button *> ESP32Button::_instanceMap;

ESP32Button::ESP32Button(const HardwarePinConfig &config) : ButtonBase(config) {
  pinMode(_pinNumber, INPUT_PULLUP);
  _debouncer.attach(_pinNumber);
  _debouncer.interval(50); // Set debounce interval
  _instanceMap[_pinNumber] = this;
  attachInterrupt(digitalPinToInterrupt(_pinNumber), ESP32Button::handleInterrupt,
                  CHANGE);
  std::string logMessage = "Button: " + id() + " initialized.";
  Logger::info(logMessage);
  setInitialized(true);
}

void ESP32Button::updatePressedState() {
  Logger::info("Button: " + _id + " updatePressedState");
  if (_debouncer.rose()) {
    if (_onPressCallback) {
      _onPressCallback(_id);
    }
  }
}

void IRAM_ATTR ESP32Button::handleInterrupt() {
  for (auto &kv : _instanceMap) {
    kv.second->_debouncer.update();
  }
}

#endif // PLATFORM_ESP32
