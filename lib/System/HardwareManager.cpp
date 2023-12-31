// HardwareManager.cpp

#include "HardwareManager.h"

#include "ConfigManager.h"
#include "Error.h"
#include "HardwareComponent.h"
#include "HardwareFactory.h"
#include "HardwareInitializer.h"

HardwareManager::HardwareManager(std::shared_ptr<ConfigManager> configManager)
    : _configManager(configManager) {}

Error HardwareManager::initialize() {
  auto hardwareFactory = HardwareFactory::getHardwareFactory();
  HardwareInitializer initializer(_configManager, hardwareFactory);

  Error initError = initializer.initialize(_components);
  if (initError) {
    Logger::error(
        "[HardwareManager] Hardware initialization failed with error: " +
        Error::getFormattedMessage(initError.code()));
    return initError;
  }

  Logger::info(
      "[HardwareManager] All hardware components initialized successfully");
  return Error::OK;
}

bool HardwareManager::isComponentInitialized(
    const std::string &componentId) const {
  // Iterate through all components in the _components map
  for (const auto &pair : _components) {
    const auto &id = pair.first;
    const auto &component = pair.second;
    // Check if the component type matches and whether it is initialized
    if (id == componentId && component->isInitialized()) {
      return true;
    }
  }
  return false;
}
