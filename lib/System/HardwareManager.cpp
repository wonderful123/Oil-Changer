// HardwareManager.cpp
#include "HardwareManager.h"
#include "HardwareInitializer.h"

HardwareManager::HardwareManager(std::shared_ptr<ConfigManager> configManager)
    : _configManager(configManager) {}

Error HardwareManager::initialize() {
  _initializer = std::make_shared<HardwareInitializer>(
      _configManager, HardwareFactory::getHardwareFactory());
  Error initResult = _initializer->initializeHardware();
  if (initResult == Error::OK) {
    _components = _initializer->getHardwareComponents();
    _displays = _initializer->getDisplays();
    Logger::info(
        "[HardwareManager] All hardware components initialized successfully");
  } else {
    Logger::error("[HardwareManager] Hardware initialization failed");
  }
  return initResult;
}

std::map<std::string, std::shared_ptr<IDisplay>>
HardwareManager::getDisplays() {
  return _displays;
}