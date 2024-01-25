// ESP32SerialDisplay.h
#pragma once

#include "DisplayBase.h"
#include "ISerial.h"
#include <memory>
#include <string>

class ESP32SerialDisplay : public DisplayBase {
public:
  // Now the constructor accepts an ISerial object and initializes IDisplay with
  // the config.
  ESP32SerialDisplay(std::shared_ptr<ISerial> serial,
                     const std::string &displayId);
  virtual ~ESP32SerialDisplay();
  void displayMessage(const std::string &data) override;

private:
  std::shared_ptr<ISerial> _serial; // Holds the serial interface.
};
