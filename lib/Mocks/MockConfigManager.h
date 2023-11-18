#pragma once

#include "ConfigManager.h"
#include <gmock/gmock.h>

class MockConfigManager : public ConfigManager {
public:
  explicit MockConfigManager(IFileHandler *fileHandler)
      : ConfigManager(fileHandler) {}

  MOCK_METHOD(std::shared_ptr<HardwareConfig>, getHardwareConfig, (),
              (const, override));
};