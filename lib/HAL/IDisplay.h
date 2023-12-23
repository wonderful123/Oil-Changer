#pragma once

#include <string>

class IDisplay {
 public:
  virtual ~IDisplay() {}

  // Display data on the screen
  virtual void displayData(const std::string &data) = 0;
  virtual std::string getId() const = 0;
};
