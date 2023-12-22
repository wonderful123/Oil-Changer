#pragma once

#include <string>

#include "ICommunicationInterface.h"
#include "ISerial.h"

class ISerial;

class SerialCommunication : public ICommunicationInterface {
 private:
  ISerial &_serial;

 public:
  explicit SerialCommunication(ISerial &serial) : _serial(serial) {}

  void send(const std::string &message) override {
    for (char c : message) {
      _serial.write(static_cast<uint8_t>(c));
    }
    _serial.write(static_cast<uint8_t>('\n'));  // For newline
  }

  std::string receive() override {
    std::string receivedData;
    while (_serial.available() > 0) {
      char ch = static_cast<char>(_serial.read());
      receivedData += ch;
    }
    return receivedData;
  }
};
