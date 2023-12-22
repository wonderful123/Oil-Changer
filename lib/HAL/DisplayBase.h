#pragma once

#include <string>

#include "ICommunicationInterface.h"
#include "IDisplay.h"

class DisplayBase : public IDisplay {
 private:
  ICommunicationInterface &_commInterface;
  std::string _displayId;

 public:
  DisplayBase(std::shared_ptr<IMediator> mediator,
              ICommunicationInterface &commInterface,
              const std::string &displayId)
      : IDisplay(mediator),
        _commInterface(commInterface),
        _displayId(displayId) {}

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
