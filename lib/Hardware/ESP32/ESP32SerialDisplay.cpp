// ESP32SerialDisplay.cpp
#include "ESP32SerialDisplay.h"
#include "HardwarePinConfig.h"
#include <Arduino.h>

ESP32SerialDisplay::ESP32SerialDisplay(std::shared_ptr<ISerial> serial,
                                       const std::string &displayId)
    : _serial(serial), _displayId(displayId) {
  // Ensure the serial interface is valid before attempting to begin
  // communication.
  if (!_serial) {
    throw std::invalid_argument("Serial interface is required");
  }
  _serial->begin();
}

ESP32SerialDisplay::~ESP32SerialDisplay() {
  if (_serial) {
    _serial->end(); // Ensure to end the serial communication properly
  }
}

void ESP32SerialDisplay::displayMessage(const std::string &data) {
  _serial->write(data); // Send messages over UART via the ISerial interface.
}

std::string ESP32SerialDisplay::getId() const { return _displayId; }