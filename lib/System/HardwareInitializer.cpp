// HardwareInitializer.cpp
#include "HardwareInitializer.h"
#include "ConfigManager.h"
#include "HardwareConfig.h"
#include "HardwareFactory.h"
#include "HardwarePinConfig.h"
#include "ICommunicationInterface.h"
#include "Logger.h"

HardwareInitializer::HardwareInitializer(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory)
    : _configManager(configManager), _hardwareFactory(hardwareFactory) {}

Error HardwareInitializer::initializeHardware() {
  Logger::info("[HardwareInitializer] Loading hardware configuration...");
  auto hardwareConfig =
      _configManager->getConfig<HardwareConfig>(ConfigType::HARDWARE);
  if (!hardwareConfig) {
    logAndReturnError(
        "[HardwareInitializer] Failed to load hardware configuration.", "",
        Error::ConfigManagerHardwareConfigNotFound);
  }

  for (const auto &pinConfig : hardwareConfig->getHardwarePinConfigs()) {
    Error error = initializeComponent(pinConfig);
    if (error)
      return error;
  }

  for (const auto &displayConfig : hardwareConfig->getDisplayConfigs()) {
    Error error = initializeDisplay(displayConfig);
    if (error)
      return error;
  }

  return Error::OK;
}

Error HardwareInitializer::initializeComponent(
    const HardwarePinConfig &config) {
  auto component = _hardwareFactory->createComponent(config);
  if (!component || !component->isInitialized()) {
        logAndReturnError("[HardwareInitializer] Component creation failed: ",
                          config.id, Error::HardwareComponentCreationFailure);
  }
  _hardware[config.id] = component;
  return Error::OK;
}

Error HardwareInitializer::initializeDisplay(const DisplayConfig &config) {
  auto component = getHardwareComponentById(config.interfaceId);
  if (!component) {
    return logAndReturnError(
        "[HardwareInitializer] Component not found: ", config.interfaceId,
        Error::HardwareDisplayCreationFailure);
  }

  const auto communicationInterface =
      std::static_pointer_cast<ICommunicationInterface>(component);
  if (!communicationInterface) {
    return logAndReturnError(
        "[HardwareInitializer] Component is not a communication interface: ",
        config.interfaceId, Error::HardwareDisplayCreationFailure);
  }

  auto display =
      _hardwareFactory->createDisplay(config, communicationInterface);
  if (!display) {
    return logAndReturnError("[HardwareInitializer] Display creation failed: ",
                             config.id, Error::HardwareDisplayCreationFailure);
  }

  _displays[config.id] = display;
  return Error::OK;
}

std::map<std::string, std::shared_ptr<HardwareComponent>>
HardwareInitializer::getHardwareComponents() const {
  return _hardware;
}

std::map<std::string, std::shared_ptr<IDisplay>>
HardwareInitializer::getDisplays() const {
  return _displays;
}

std::shared_ptr<HardwareComponent>
HardwareInitializer::getHardwareComponentById(const std::string &id) const {
  auto it = _hardware.find(id);
  return it != _hardware.end() ? it->second : nullptr;
}

Error HardwareInitializer::logAndReturnError(const std::string &message,
                                             const std::string &id,
                                             Error error) {
  Logger::error(message + id);
  return error;
}
