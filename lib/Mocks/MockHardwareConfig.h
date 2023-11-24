#pragma once

#include "HardwarePinConfig.h"
#include "HardwareConfig.h"
#include "IFileHandler.h"
#include <gmock/gmock.h>
#include <vector>

class MockHardwareConfig : public HardwareConfig {
public:
  explicit MockHardwareConfig(IFileHandler *fileHandler)
      : HardwareConfig(fileHandler) {}

  // Mock the getHardwarePinConfigs() method
  MOCK_METHOD(const std::vector<HardwarePinConfig> &, getHardwarePinConfigs, (),
              (const, override));
};
