#pragma once

#include "ConfigManager.h"
#include <gmock/gmock.h>
#include <memory>

class MockConfigManager : public ConfigManager {
public:
  explicit MockConfigManager(IFileHandler *fileHandler)
      : ConfigManager(fileHandler) {}

  MOCK_METHOD(std::shared_ptr<HardwareConfig>, getHardwareConfig, (),
              (const, override));
};
