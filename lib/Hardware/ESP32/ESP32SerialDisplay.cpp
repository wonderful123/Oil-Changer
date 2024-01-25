// ESP32SerialDisplay.cpp
#include "ESP32SerialDisplay.h"
#include "HardwarePinConfig.h"
#include "Logger.h"
#include <Arduino.h>

ESP32SerialDisplay::ESP32SerialDisplay(std::shared_ptr<ISerial> serial,
                                       const std::string &displayId)
    : _serial(serial), DisplayBase(displayId) {
  // Ensure the serial interface is valid before attempting to begin
  // communication.
  if (!_serial) {
    Logger::error("[ESP32SerialDisplay] Serial interface is required");
  }
  _serial->begin();
}

ESP32SerialDisplay::~ESP32SerialDisplay() {
  if (_serial) {
    _serial->end(); // Ensure to end the serial communication properly
  }
}

void ESP32SerialDisplay::displayMessage(const std::string &data) {
  std::string formattedMessage = "<" + _id + ";" + data + ">";
  _serial->write(
      formattedMessage); // Send message over UART via ISerial interface.
}
