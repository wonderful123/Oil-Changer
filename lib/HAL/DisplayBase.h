#pragma once

#include "IDisplay.h"

class DisplayBase : public IDisplay {
 public:
  virtual ~DisplayBase() {}

  virtual void displayData(const std::string &data) = 0;

  std::string getId() const override { return id; }

 protected:
  DisplayBase(const std::string &displayId) : id(displayId) {}

 private:
  std::string id;
};