// MockConfigManager.h
#pragma once

#include "ConfigManager.h"
#include <gmock/gmock.h>
#include <memory>

// Forward declarations
class IFileHandler;

class MockConfigManager : public ConfigManager {
public:
  // Use the constructor of ConfigManager
  MockConfigManager(std::shared_ptr<IFileHandler> fileHandler)
      : ConfigManager(fileHandler) {}

  // Mock the methods from ConfigManager
  MOCK_METHOD(std::shared_ptr<HardwareConfig>, getHardwareConfig, (),
              (const, override));
  MOCK_METHOD(std::shared_ptr<InteractionSettingsConfig>,
              getInteractionSettingsConfig, (), (const));
  MOCK_METHOD(Error, loadConfig, (const std::string &configType));
  MOCK_METHOD(void, receiveEvent,
              (EventType eventType, const EventData *eventData), (override));
};