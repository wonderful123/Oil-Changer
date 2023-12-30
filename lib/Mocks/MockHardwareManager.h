#pragma once

#include "HardwareManager.h"
#include "Mediator/IMediator.h"
#include <functional>
#include <map>
#include <memory>
#include <tinyfsm.hpp>
#include <unordered_map>

class ButtonController;
class ConfigManager;
class HardwareComponent;
class HardwareFactory;
class HardwarePinConfig;

class MockHardwareManager : public HardwareManager {
public:
  MockHardwareManager(std::shared_ptr<IMediator> mediator,
                      std::shared_ptr<ConfigManager> configManager,
                      std::shared_ptr<HardwareFactory> hardwareFactory,
                      std::shared_ptr<ButtonController> buttonController)
      : HardwareManager(mediator, configManager, hardwareFactory,
                        buttonController) {}

  // Mock the initializeHardware method
  MOCK_METHOD(void, initializeHardware, (), (override));

  // Mock the isComponentInitialized method
  MOCK_METHOD(bool, isComponentInitialized, (const std::string &componentId),
              (const, override));

  // Mock the getComponentById method
  MOCK_METHOD(std::shared_ptr<HardwareComponent>, getComponentById,
              (const std::string &id), (const, override));

  // Mock the triggerBuzzer method
  MOCK_METHOD(void, triggerBuzzer, (), (override));

  // Mock the updateBuzzerSettings method
  MOCK_METHOD(void, updateBuzzerSettings, (), ());

  // Mock the receiveEvent method
  MOCK_METHOD(void, receiveEvent,
              (EventType eventType, const EventData *eventData), (override));
};
