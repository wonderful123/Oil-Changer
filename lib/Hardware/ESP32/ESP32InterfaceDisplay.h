#pragma once

#ifdef PLATFORM_ESP32

#include "DisplayBase.h"
#include "ICommunicationInterface.h"
#include <string>

class ESP32InterfaceDisplay : public DisplayBase {
public:
  // Constructor
  ESP32InterfaceDisplay(ICommunicationInterface &commInterface,
                        const std::string &displayId)
      : DisplayBase(commInterface, displayId) {}

  // Destructor
  virtual ~ESP32InterfaceDisplay() {}
};

#endif // PLATFORM_ESP32