#pragma once

#ifdef PLATFORM_ESP32

#include <string>

#include "DisplayBase.h"

class ICommunicationInterface;

class ESP32InterfaceDisplay : public DisplayBase {
 public:
  // Constructor
  ESP32InterfaceDisplay(ICommunicationInterface &commInterface,
                        const std::string &displayId)
      : DisplayBase(commInterface, displayId) {}

  // Destructor
  virtual ~ESP32InterfaceDisplay() {}

  void setMediator(std::shared_ptr<IMediator> mediator) override {
    // Implementation
  }

  void receiveEvent(EventType eventType, const std::string &data) override {
    // Implementation
  }

  void displayData(const std::string &data) override {
    // Implementation
  }
};

#endif  // PLATFORM_ESP32