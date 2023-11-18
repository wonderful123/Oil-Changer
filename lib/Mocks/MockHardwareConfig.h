#pragma once

#include "GpioPinConfig.h"
#include "HardwareConfig.h"
#include "IFileHandler.h"
#include <gmock/gmock.h>
#include <vector>

class MockHardwareConfig : public HardwareConfig {
public:
  explicit MockHardwareConfig(IFileHandler *fileHandler)
      : HardwareConfig(fileHandler) {}

  // Mock the getGpioConfigs() method
  MOCK_METHOD(const std::vector<GpioPinConfig> &, getGpioConfigs, (),
              (const, override));
};
