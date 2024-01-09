// ESP32SerialDisplay.h
#pragma once

#include "IDisplay.h"
#include "ISerial.h"
#include <memory>
#include <string>

class ESP32SerialDisplay : public IDisplay {
public:
  // Now the constructor accepts an ISerial object and initializes IDisplay with
  // the config.
  ESP32SerialDisplay(std::shared_ptr<ISerial> serial,
                     const std::string &displayId);
  virtual ~ESP32SerialDisplay();
  void displayMessage(const std::string &data) override;
  std::string getId() const override;

private:
  std::shared_ptr<ISerial> _serial; // Holds the serial interface.
  std::string _displayId;           // Unique identifier for the display.
};
