// ESP32SerialDisplay.h
#pragma once

#ifdef PLATFORM_ESP32

#include <string>

#include "DisplayBase.h"
#include "ESP32Serial.h"
#include "HardwarePinConfig.h"
#include "ICommunicationInterface.h"

class ESP32SerialDisplay : public DisplayBase, public ICommunicationInterface {
 private:
  ICommunicationInterface &communicationInterface;
  ESP32Serial _serial;           // Holds the ESP32Serial object

 public:
  ESP32SerialDisplay(const std::string &displayId,
                     ICommunicationInterface &commInterface,
                     const HardwarePinConfig &config)
      : DisplayBase(displayId),
        communicationInterface(commInterface),
        _serial(config) {
    // Initialize the _serial communication with default baud rate
    _serial.begin(115200);
  }

  void displayData(const std::string &data) override { _serial.write(data); }

  void send(const std::string &message) override {
    displayData(message);  // Send the message to the display
  }

  std::string receive() override {
    std::string receivedData;
    while (_serial.available() > 0) {
      receivedData += static_cast<char>(_serial.read());
    }
    return receivedData;
  }

  ~ESP32SerialDisplay() {
    _serial.end();  // Ensure the _serial communication is properly ended
  }
};

#endif  // PLATFORM_ESP32