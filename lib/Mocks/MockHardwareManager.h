#pragma once

#ifdef PLATFORM_NATIVE

#include "HardwareComponent.h"
#include "HardwareManager.h"
#include "HardwarePinConfig.h"
#include <gmock/gmock.h>
#include <memory>

class MockHardwareManager : public HardwareManager {
public:
  MockHardwareManager(std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory,
                      std::shared_ptr<ButtonController> buttonController)
      : HardwareManager(std::move(configManager), std::move(hardwareFactory),
                        std::move(buttonController)) {}

  MOCK_METHOD(void, initializeHardware, (), (override));
  MOCK_METHOD(void, update, (), (override));
  MOCK_METHOD(void, onButtonEvent, (const std::string &buttonId, bool pressed),
              (override));
  MOCK_METHOD(void, triggerBuzzer, (), (override));
  MOCK_METHOD(bool, initializeComponent, (const HardwarePinConfig &config),
              (override));

  MOCK_METHOD(bool, isComponentInitialized, (const std::string &componentId),
              (override, const));
  MOCK_METHOD(std::shared_ptr<HardwareComponent>, getComponentById,
              (const std::string &id), (override, const));
};

#endif // PLATFORM_NATIVE