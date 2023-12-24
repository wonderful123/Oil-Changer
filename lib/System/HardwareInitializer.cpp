#include "HardwareInitializer.h"

HardwareInitializer::HardwareInitializer(
    std::shared_ptr<ConfigManager> configManager,
    std::shared_ptr<HardwareFactory> hardwareFactory)
    : _configManager(configManager), _hardwareFactory(hardwareFactory) {}

Error HardwareInitializer::initialize(
    std::map<std::string, std::shared_ptr<HardwareComponent>>& components) {
  Logger::info("[HardwareInitializer] Starting initialization...");

  auto hardwareConfig = _configManager->getHardwareConfig();
  if (!hardwareConfig) {
    Logger::error("[HardwareInitializer] Configuration not available.");
    return Error::ConfigManagerHardwareConfigNotFound;
  }

  for (const auto& pinConfig : hardwareConfig->getHardwarePinConfigs()) {
    std::shared_ptr<HardwareComponent> component;
    auto error = initializeComponent(pinConfig, component);
    if (error) {
      Logger::error(
          "[HardwareInitializer] Initialization error for component: " +
          pinConfig.id);
      return error;
    }
    components[pinConfig.id] = component;
    Logger::info("[HardwareInitializer] Initialized: " + pinConfig.id);
  }

  return Error::OK;
}

Error HardwareInitializer::initializeComponent(
    const HardwarePinConfig& config,
    std::shared_ptr<HardwareComponent>& component) {
  component = _hardwareFactory->createComponent(config);
  if (!component) {
    Logger::error("[HardwareInitializer] Component creation failed: " +
                  config.id);
    return Error::HardwareComponentCreationFailure;
  }

  return Error::OK;
}