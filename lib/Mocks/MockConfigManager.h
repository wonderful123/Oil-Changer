#pragma once

#ifdef PLATFORM_NATIVE

#include "ConfigManager.h"
#include "Error.h"
#include "HardwareConfig.h"
#include <gmock/gmock.h>
#include <memory>

class MockConfigManager : public ConfigManager {
public:
  explicit MockConfigManager(IFileHandler *fileHandler)
      : ConfigManager(fileHandler) {}

  // Mock the getHardwareConfig method
  MOCK_METHOD((std::shared_ptr<HardwareConfig>), getHardwareConfig, (),
              (const, override));

  // Mock the loadConfig method
  MOCK_METHOD(Error, loadConfig, (const std::string &));
};

#endif // PLATFORM_NATIVE
