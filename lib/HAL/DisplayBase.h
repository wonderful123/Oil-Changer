// DisplayBase.h
#pragma once

#include "IDisplay.h"
#include <string>

class DisplayBase : public IDisplay {
public:
  explicit DisplayBase(const std::string &displayId) : _id(displayId) {};
  ;
  virtual ~DisplayBase() = default;

  virtual void displayMessage(const std::string &data) = 0;
  std::string getId() const override { return _id; }

private:
  std::string _id; // Unique identifier for the display.
};