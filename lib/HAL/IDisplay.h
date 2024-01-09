// IDisplay.h
#pragma once

#include <string>

class IDisplay {
public:
  virtual ~IDisplay() = default;

  // Display a message on the screen.
  virtual void displayMessage(const std::string &data) = 0;

  // Get the unique identifier of the display.
  virtual std::string getId() const = 0;
};