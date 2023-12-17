#pragma once

#include "IDisplay.h"
#include <string>

class ICommunicationInterface;

class DisplayBase : public IDisplay {
private:
  ICommunicationInterface &_commInterface;
  std::string _displayId;

public:
  explicit DisplayBase(ICommunicationInterface &commInterface,
                       const std::string &displayId)
      : _commInterface(commInterface), _displayId(displayId) {}

  virtual ~DisplayBase() {}

  virtual void displayData(const std::string &data) override {
    auto separatorPos = data.find(':');
    if (separatorPos != std::string::npos) {
      std::string targetDisplay = data.substr(0, separatorPos);
      if (targetDisplay == _displayId) {
        std::string message = data.substr(separatorPos + 1);
        _commInterface.send(message);
      }
    }
  }

  // ... Update method implementation, etc.
};
